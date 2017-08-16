#include "WL.hpp"

/*
 * functions in class wl (Wang-Landau)
 */


bool wl::checkFlatness()
{
  bool FLAT = true;
  double averageH = H.meanHeight();

  if( std::any_of(H.begin(), H.end(), [&averageH](auto B){return (double)B.counter < averageH*prms::wl_flatness;}) )
    FLAT = false;

  return FLAT;

}

/***************************************************************************/

void wl::updateF()
{
  // update factor and clear histogram H:
  F /= 2.0;
  H.reset(0);

  std::cout << "   update F -> F/2 = " << F << std::endl;
}

/***************************************************************************/

void wl::do_simulation()
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
    // check flatness:
    if( stepper % prms::wl_checkflatness == 0 && stepper > 0){
      if( checkFlatness() )
        updateF();
    }
    // check convergence:
    if( F < prms::wl_convcrit ) CONVERGED = true;
    stepper ++;
  }
  if( CONVERGED ){
    std::cout << "   converged after " << stepper << " cycles\n";
  }else{
    std::cout << "   NOT converged after " << stepper << " cycles\n";
  }

}

/***************************************************************************/

// void wl::save_data()
// {
//   if( prms::VERBOSE ) std::cout << "    saving data ...\n";
//   // H.print_to_file(prms::output_key+"-WL-H.data");
//   S.print_to_file(prms::output_key+"-WL-S.data");
//   normS.print_to_file(prms::output_key+"-WL-normS.data");
//
// }

/***************************************************************************/
