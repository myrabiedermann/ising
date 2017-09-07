#pragma once

#include "../global.hpp"
#include "../utility/randomnumbers.hpp"
#include "../utility/utility.hpp"
#include "spin.hpp"

class Spinsystem {

    const SIZE     size;
    const REAL     J;
    const bool     CONSTRAINED;
    const SIZE     totalnumber;

    std::vector<Spin>  spins {};
    REAL               Hamiltonian = 0;
    std::vector<std::reference_wrapper<Spin>> lastFlipped {};

    // functions:
    REAL   local_energy(const Spin&) const;
    SIGNED JijwithoutJ(const SPINTYPE, const SPINTYPE) const;

public:
    // constructor:
    Spinsystem(const SIZE&, const REAL&, const bool& = false);
    // functions:
    void flip();
    void flip_back();
    

    inline const decltype(spins)& get_spins() const { return spins; };
    inline auto  get_edge()  const {return size;}
    inline auto  get_totalnumber()  const {return totalnumber;}
    inline auto  get_Hamiltonian()  const {return Hamiltonian;}

    void print(std::ostream & ) const;
};
