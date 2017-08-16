#include "Testimator.hpp"

/*
 * functions in struct Testimator
 */



Testimator::Testimator(Eigen::MatrixXi& _C)
 : C (_C)
 , Ci (C.rowwise().sum())
{
}

/***************************************************************************/

void Testimator::initial_probabilities()
{
  pi.resize(C.rows());
  pi.setZero();

  // computation of initial probabilites according to Noe et al 2015 / eq. 2.26 in MA thesis
  for(SIZE i=0; i<C.rows(); ++i){
    for(SIZE j=0; j<C.cols(); ++j){
      pi(i) += ( C(i,j) + C(j,i) );
    }
  }
  // normalisation:
  pi /= (Ci.sum() * 2.0);

#ifndef NDEBUG
  // print intial probabilities to standard output:
  std::cout << "initial  probabilities: \n";
  std::cout << pi << "\n";
#endif

}

/***************************************************************************/

void Testimator::maximumLikelihood()
{
  Eigen::VectorXd x_old, x_new;
  SIZE stepper = 0;
  SIZE maxSteps = 5e6;
  bool CONVERGED = false;

  // estimate initial probabilities:
  initial_probabilities();
  x_new = pi;

  // compute maximum likelihood estimates for equilibrium probabilities
  // according to TRAM, Noe 2015
  while( stepper < maxSteps and ! CONVERGED) {
    x_old = x_new;
    // update all x_i:
    x_new.setZero();
    for(auto i=0; i<x_new.rows(); ++i){
        for(auto j=0; j<x_new.rows(); ++j){
            x_new(i) += ( C(i,j) + C(j,i) ) / (Ci(i)/x_old(i) + Ci(j)/x_old(j));	// eq. 37 / eq.2.24 in MA thesis
        }
    }
    // normalise the estimates:
    x_new /= x_new.sum();
    // check for convergence:
    if( convergence(x_old, x_new) ){
        CONVERGED = true;
    }
    stepper ++;
  }
  if( CONVERGED )
    std::cout << "MSM estimation converged after " << stepper << " iterations \n";
  else
    std::cout << "WARNING: MSM not converged !\n";

  // save final probabilities:
  pi = x_new;

  // calc T from final probabilities:
  calc_T();

}

/***************************************************************************/

bool Testimator::convergence(const Eigen::VectorXd& _v1, const Eigen::VectorXd& _v2)
{
    Eigen::VectorXd difference = _v2 - _v1;
    return difference.norm() < 1e-10 ? true : false;
}

/***************************************************************************/

void Testimator::calc_T()
{
    // calc T from stationary distribution pi
    T.resize(pi.rows(), pi.rows());
    // eq. 2.28 in MA thesis:
    for( SIZE i = 0; i < pi.rows(); ++i ){
        for( SIZE j = 0; j < pi.rows(); ++j ){
            T(i,j) = static_cast<double>( C(i,j) + C(j,i) ) * pi(j)
                   / ( static_cast<double>(Ci(i))*pi(j) + static_cast<double>(Ci(j))*pi(i) );
        }
        // for( unsigned int j = 0; j < pi.rows(); ++j )
          // T(i,j) /= T.row(i).sum();
    }

}

/***************************************************************************/
