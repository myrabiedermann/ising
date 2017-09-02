#pragma once

#include "../global.hpp"
#include "../utility/randomnumbers.hpp"
#include "../utility/utility.hpp"
#include "spin.hpp"

class spinsys {

    const SIZE     size;
    const REAL     J;
    const bool     CONSTRAINED;
    const SIZE     totalnumber;

    std::vector<spin>  spins;
    REAL               Hamiltonian;
    std::vector<std::reference_wrapper<spin>> lastFlipped;

    // functions:
    REAL   local_energy(const spin&) const;
    SIGNED JijwithoutJ(const SPINTYPE, const SPINTYPE) const;

public:
    // constructor:
    spinsys(const SIZE&, const REAL&, const bool& = false);
    // functions:
    void flip();
    void flip_back();

    inline auto  get_totalnumber()  const {return totalnumber;}
    inline auto  get_Hamiltonian()  const {return Hamiltonian;}

    void print(std::ostream & ) const;
};
