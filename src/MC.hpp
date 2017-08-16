#pragma once

#include "global.hpp"
#include "utility/randomnumbers.hpp"
#include "input/parameters.hpp"
#include "system/spinsys.hpp"

/*
 * class mc
 */


class mc {

    spinsys             spinsystem;
    std::vector<REAL>   trajectory;

public:
    mc();

    void  do_metropolis();

    void  save_trj(const PATH&);

    const auto& get_trajectory() const { return trajectory; }
    auto&       get_trajectory()       { return trajectory; }

};
