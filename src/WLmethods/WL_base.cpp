#include "WL_base.hpp"

/*
 * functions in class wlBase
 */


// constructor:
wlBase::wlBase()
  : spinsystem(prms::systemsize, prms::J, prms::CONSTRAINED)
  , F (1)
  , CONVERGED (false)
{

  // allocate histograms:
  auto vec = spinsystem.possibleHwithoutJ();
  for(auto& V: vec){
    H.add_bin(V);
    S.add_bin(V, 1);
  }

// debug printing:
#ifndef NDEBUG
    std::cout << "inital: H = " << spinsystem.get_Hamiltonian() << "\n";
    std::cout << "        H/J = " << spinsystem.get_HwithoutJ() << "\n";
    std::cout << spinsystem;
#endif

}

/***************************************************************************/

void wlBase::normalise_data()
{
  std::cout << "   normalising data ...\n";
  double groundState;
  groundState = S.get_data(S.minimum());
  std::for_each(S.begin(), S.end(), [&](auto& B){normS.add_bin(B.value, B.counter - groundState);});

}

/***************************************************************************/

void wlBase::save_data() const
{
  std::cout << "    saving data ...\n";
  // H.print_to_file(prms::output_key+"-WL-H.data");
  S.print_to_file(prms::output_key+"-WL-S.data");
  normS.print_to_file(prms::output_key+"-WL-normS.data");

}

/***************************************************************************/
