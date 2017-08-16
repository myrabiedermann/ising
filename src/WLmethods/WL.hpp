#pragma once

#include "WL_base.hpp"


/*
 * class "wl"
 */

class wl : public wlBase {

  // functions:
  bool checkFlatness();
  void updateF();

public:
  using wlBase::wlBase;
  void do_simulation();
  // void save_data();

};
