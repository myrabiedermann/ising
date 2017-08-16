#pragma once

#include "global.hpp"
#include "utility/randomnumbers.hpp"
#include "input/parameters.hpp"
#include "system/spinsys.hpp"
#include "utility/pngmaker.hpp"

/*
 * class mc
 */


class mc {

  spinsys             spinsystem;
  std::vector<SIGNED> HwithoutJ_trj;
  SIZE                print_frequency;

public:
  mc();

  void  do_metropolis(const SIZE&, const SIZE& = prms::mc_printfreq);

  void  save_trj(const boost::filesystem::path&);

  auto& get_HwithoutJ_trj() const { return HwithoutJ_trj; }
  auto& get_HwithoutJ_trj()       { return HwithoutJ_trj; }

};
