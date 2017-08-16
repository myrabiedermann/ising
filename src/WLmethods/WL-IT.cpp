#include "WL-IT.hpp"

/*
 * functions in class wl-it (Inverse Time Wang Landau algorithm)
 */


bool wl_it::checkOccupancy()
{

  return std::all_of(H.begin(), H.end(), [](auto B){return B.counter != 0;});

}

/***************************************************************************/

void wl_it::updateF()
{
  double invtime = (double) H.num_bins() / stepper;

  if( ! SWITCH ){       // F -> F/2 modification
    if( stepper % prms::it_checkOccupancy == 0 and stepper > 0 and checkOccupancy() ){
      F /= 2.0;
      H.reset(0);
      std::cout << "   @ " << stepper << " update F -> F/2 = " << F << "\n";
    }
    // check for SWITCH:
    if( F <= invtime and stepper > H.num_bins() ){
      SWITCH = true;
      F = invtime;
      std::cout << "   @ " << stepper << " switch to F -> 1/t update, F = " << F << "\n";
    }
  }else{                // F -> 1/t modification
    if( stepper % H.num_bins() )
      F = invtime;
  }

}

/***************************************************************************/

void wl_it::do_simulation()
{

  CONVERGED = false;
  double E_old, E_new, S_old, S_new;  // attention: E_old/new = H/J or HwithoutJ!
  bool accepted = false;

  // loop until converged:
  while( !CONVERGED and stepper < 4e9 ){
    // E_old:
    E_old = spinsystem.get_HwithoutJ();
    S_old = S.get_data(E_old);
    // flip random spin:
    spinsystem.flip();
    E_new = spinsystem.get_HwithoutJ();
    S_new = S.get_data(E_new);
    // check acceptance criterion:
    if( S_new < S_old or randomDouble(0.0,1.0) < exp(S_old-S_new) ){
      accepted = true;
    }else {
      accepted = false;
      // debugging / verbose printing:
  #ifndef NDEBUG
        std::cout << "cycle " << stepper << ": \n";
        std::cout << "Eold: " << E_old << " Sold: " << S_old << "\n";
        std::cout << "Enew: " << E_new << " Snew: " << S_new << "\n";
        if( accepted ) {
          std::cout << "... flip accepted \n";
          std::cout << spinsystem;
        }else std::cout << "... flip rejected \n";
  #endif
      // flip back because move not accepted:
      spinsystem.flip_back();
      E_new = E_old;
      S_new = S_old;
    }
    // update histograms:
    H.add_data(E_new);
    S.add_data(E_new, F);

    // update F:
    updateF();
    // check convergence:
    if( F < prms::it_convcrit ) CONVERGED = true;
    stepper ++;
  }
  if( CONVERGED ){
    std::cout << "   converged after " << stepper << " cycles\n";
  }else{
    std::cout << "   NOT converged after " << stepper << " cycles\n";
  }

}

/***************************************************************************/

// void wl_it::save_data()
// {
//   if( prms::VERBOSE ) std::cout << "    saving data ...\n";
//   // H.print_to_file(prms::output_key+"-IT-H.data");
//   S.print_to_file(prms::output_key+"-IT-S.data");
//   normS.print_to_file(prms::output_key+"-IT-normS.data");
//
// }

/***************************************************************************/
