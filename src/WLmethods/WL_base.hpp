#pragma once

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <algorithm>
#include "../utility/randomnumbers.hpp"
#include "../system/spinsys.hpp"
#include "../input/parameters.hpp"
#include "histogram.hpp"


/*
 * class "wlBase"
 */

class wlBase {

protected:
  spinsys             spinsystem;
  histogram<int>      H;
  histogram<double>   S;
  histogram<double>   normS;
  double              F;
  bool                CONVERGED;
  long unsigned int   stepper = 0;
  // functions:
  virtual void updateF() = 0;

public:
  wlBase();
  virtual void do_simulation() = 0;
  void normalise_data();
  virtual void save_data() const;

  inline auto  get_H() const {return H;};
  inline auto  get_S() const {return S;};
  inline auto  get_F() const {return F;};

};
