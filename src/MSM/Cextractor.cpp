#include "Cextractor.hpp"

/*
 * functions in struct Cextractor
 */


void Cextractor::extract(const std::vector<TRAJECTORY>& _trajs, const SIZE& _tau, const std::vector<SIGNED>& _possO)
{
    possibleOrders = _possO;

   // first initialisation of countMatrix:
   countMatrix.conservativeResize(1,1);
   countMatrix.setZero();
   translator.push_back(_trajs[0][0]);

   // loop through all trajectories:
   for(auto& TRJ: _trajs){

     // allocate vector to save history:
     std::vector<int> history;
     history.resize(_tau+1);

     // loop through each trajectory and extract transitions in window-slide-method
   	 auto trajlength = TRJ.size();
     for(SIZE t = 0; t < trajlength; ++t){

        if(t >= _tau){

            // update history:
            if(t > _tau)
                for(SIZE k=0; k<_tau; ++k) history[k] = history[k+1];
            history[_tau] = TRJ[t];

            // update countmatrix
            SIGNED index1, index2;
            // first: search if these order parameters are already registered (history[0] -> history[_tau])
            // search for history[0]:
            auto it1 = std::find( std::begin(translator), std::end(translator), history[0] );
            if( it1 != std::end(translator) ){   // found entry for history[0]
                index1 = std::distance(std::begin(translator), it1);
            }else{                               // did not find entry for history[0]
                translator.push_back(history[0]);
                index1 = countMatrix.rows();
                countMatrix.conservativeResize( countMatrix.rows()+1, countMatrix.cols()+1 );
                countMatrix.col(index1).setZero();
                countMatrix.row(index1).setZero();
            }
            // search for history[_tau]:
            auto it2 = std::find( std::begin(translator), std::end(translator), history[_tau] );
            if( it2 != std::end(translator) ){   // found entry for history[_tau]
                index2 = std::distance(std::begin(translator), it2);;
            }else{                               // did not find entry for history[_tau]
                translator.push_back(history[_tau]);
                index2 = countMatrix.rows();
                countMatrix.conservativeResize( countMatrix.rows()+1, countMatrix.cols()+1 );
                countMatrix.col(index2).setZero();
                countMatrix.row(index2).setZero();
          }
          // then: update countMatrix
          countMatrix(index1, index2) += 1;

        }else {
          // update history:
          history[t] = TRJ[t];
        }

     }// end of t loop


   }// end of TRJ loop

   // get countMatrix_sorted:
   sortC();
   // check ergodicity:
   checkErgodicity();

}

/***************************************************************************/

void Cextractor::sortC()
{
   // sort countMatrix in increasing order of order parameter

   // first: sort translator:
   translator_sorted = translator;
   std::sort(std::begin(translator_sorted), std::end(translator_sorted));

   // second: find formula for rearrangements:
   std::vector<SIGNED> assignments(translator_sorted.size());
   for( SIZE i = 0; i < assignments.size(); ++i ){
        auto iter = std::find( std::begin(translator), std::end(translator), translator_sorted[i] );
        if( iter != std::end(translator) ){   // found entry
            assignments[i] = std::distance(std::begin(translator), iter);
        }else{
            std::cout << "something went wrong in sorting C\n";
        }
   }

   // third: rearrange countMatrix_sorted:
   countMatrix_sorted.resize(countMatrix.rows(), countMatrix.cols());;
   for(auto i = 0; i < countMatrix_sorted.rows(); ++i){
        for(auto j = 0; j < countMatrix_sorted.cols(); ++j){
            countMatrix_sorted(i,j) = countMatrix(assignments[i], assignments[j]);
        }
   }


}

/***************************************************************************/

void Cextractor::checkErgodicity()
{

    // find largest connected (=ergodic) set in translator_sorted

    // first: find difference between possibleEnergies and translator_sorted:
    auto beginning = std::find(std::begin(possibleOrders), std::end(possibleOrders), translator_sorted.front());

    std::vector<SIGNED> difference;
    std::set_difference(beginning, std::end(possibleOrders), std::begin(translator_sorted), std::end(translator_sorted),
                      std::inserter(difference, std::begin(difference)));

    std::cout << "difference: ";
    for( auto& D: difference ) std::cout << D << " ";
    std::cout << "\n";

    if( difference.size() != 0 )  // search for largest ergodic subset and adjust Cmatrix accordingly
    {
        // calculate sizes of subsets:
        std::vector<SIGNED> sizes;
        auto leftEdge = beginning;
        auto rightEdge = beginning;
        for( auto& D: difference )
        {
            auto rightEdge = std::find( std::begin(possibleOrders), std::end(possibleOrders), D ) - 1;
            sizes.push_back( std::distance(leftEdge, rightEdge) );
            leftEdge = rightEdge + 2;
        }

        // find largest subset:
        auto maxSize = std::max_element( std::begin(sizes), std::end(sizes) );

        // find left/right Edges of largest subset:
        if( maxSize == std::begin(sizes) ){
            leftEdge  = beginning;
            rightEdge = std::find( std::begin(possibleOrders), std::end(possibleOrders),
                          difference[std::distance(std::begin(sizes), maxSize)] ) - 1;

        }else{
            leftEdge  = std::find(std::begin(possibleOrders), std::end(possibleOrders),
                        difference[std::distance(std::begin(sizes), maxSize) - 1]) + 1;
            rightEdge = std::find( std::begin(possibleOrders), std::end(possibleOrders),
                          difference[std::distance(std::begin(sizes), maxSize)] ) - 1 ;
        }

        // adjust range of Cmatrix:
        auto left  = std::find( std::begin(translator_sorted), std::end(translator_sorted), *leftEdge );
        auto right = std::find( std::begin(translator_sorted), std::end(translator_sorted), *rightEdge );
        SIGNED position  = std::distance( std::begin(translator_sorted), left );
        SIGNED blocksize = std::distance( std::begin(translator_sorted), right ) + 1;
        countMatrix_ergodic = countMatrix_sorted.block(position, position, blocksize, blocksize);

        // adjust range of translator and save it directly to energies:
        energies.resize( blocksize );
        for( SIGNED i = position; i < (position + blocksize); ++i ){
            energies(i) = static_cast<double>( translator_sorted[i] );
            translator_ergodic.push_back( translator_sorted[i] );
        }

        // some debug printing:
#ifndef NDEBUG
        std::cout << "possible H/J:\n";
        for(auto& V: possibleOrders) std::cout << V << " ";
        std::cout << "\n";
        std::cout << "found H/J (when extracting C): \n";
        for(auto& V: translator_sorted) std::cout << V << " ";
        std::cout << "\n";
        std::cout << "largest set: " << *leftEdge << " - " << *rightEdge << "\n";
        std::cout << "new Cmatrix: \n " << countMatrix_ergodic << "\n";
#endif

    }  // else: nothing to do, set is ergodic

}
