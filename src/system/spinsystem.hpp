#pragma once

#include "../global.hpp"
#include "../utility/randomnumbers.hpp"
#include "../utility/utility.hpp"
#include "spin.hpp"
#include "gui/parameters_widget.hpp"



class Spinsystem {

//     SIZE     size;
//     REAL     J;
//     bool     CONSTRAINED;
//     SIZE     totalnumber;

    std::vector<Spin>  spins {};
    REAL               Hamiltonian = 0;
    std::vector<std::reference_wrapper<Spin>> lastFlipped {};
    ParametersWidget* parameters = Q_NULLPTR;

    // functions:
    REAL   local_energy(const Spin&) const;
    SIGNED JijwithoutJ(const SPINTYPE, const SPINTYPE) const;

public:
    // constructor:
    Spinsystem();
    Spinsystem(const Spinsystem&) = delete;
    void operator=(const Spinsystem&) = delete;
    ~Spinsystem();
    
    void setParameters(ParametersWidget*);
    void setup();
    
    // functions:
    void flip();
    void flip_back();
    
    inline unsigned long getWidth()  const { return parameters->getWidth(); }
    inline unsigned long getHeight() const { return parameters->getHeight(); }
    

    inline const decltype(spins)& get_spins() const { return spins; };
//     inline auto  get_edge() const {return size;}
//     inline auto  get_totalnumber() const {return totalnumber;}
    inline auto  get_Hamiltonian() const {return Hamiltonian;}

    void print(std::ostream & ) const;
};
