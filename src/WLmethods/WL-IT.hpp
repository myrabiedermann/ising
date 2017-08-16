#pragma once

#include "WL_base.hpp"

/*
 * class "wl-it"
 */

class wl_it : public wlBase {

  bool SWITCH = false;

  // functions:
  bool checkOccupancy();
  void updateF();

public:
  using wlBase::wlBase;
  void do_simulation();
  // void save_data();

};
