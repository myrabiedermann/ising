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
    const SIGNED   minHwithoutJ;
    const SIGNED   maxHwithoutJ;

    std::vector<spin>  spins;
    REAL               Hamiltonian;
    SIGNED             HwithoutJ;
    std::vector<std::reference_wrapper<spin>> lastFlipped;

    // functions:
    SIGNED local_energy(const spin&) const;

public:
    // constructor:
    spinsys(const SIZE&, const REAL&, const bool& = false);
    // functions:
    void flip();
    void flip_back();

    std::vector<int> possibleHwithoutJ();

    inline auto  get_totalnumber() const {return totalnumber;}
    inline auto  get_Hamiltonian() const {return Hamiltonian;}
    inline auto  get_HwithoutJ()   const {return HwithoutJ;}
    inline auto  get_HwithoutJmin()   const {return minHwithoutJ;}
    inline auto  get_HwithoutJmax()   const {return maxHwithoutJ;}

    int JijwithoutJ(const SPINTYPE, const SPINTYPE) const;

    void print(std::ostream & ) const;
};
