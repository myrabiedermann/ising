#pragma once

#include "lib/enhance.hpp"
#include "spin.hpp"
#include "gui/parameters_widget.hpp"
#include <cassert>
#include <numeric>
#include <ostream>



class Spinsystem 
{
    float Hamiltonian = 0;
    std::vector<Spin>  spins {};
    std::vector<std::reference_wrapper<Spin>> lastFlipped {};
    ParametersWidget* parameters = Q_NULLPTR;

    // functions:
    double local_energy(const Spin&) const;
    int JijwithoutJ(const SPINTYPE, const SPINTYPE) const;

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
    
    
    template<SPINTYPE T> constexpr int num() const;

    inline const decltype(spins)& getSpins() const { return spins; };
//     inline auto  get_edge() const {return size;}
//     inline auto  get_totalnumber() const {return totalnumber;}
    inline auto  getHamiltonian() const {return Hamiltonian;}

    void print(std::ostream & ) const;
};





template<SPINTYPE T>
constexpr inline int Spinsystem::num() const
{
    // return sum over neighbours of type T
    return std::accumulate(std::begin(spins), std::end(spins), 0, [](int i, const Spin& S) 
    {
        return S.get_type() == T ? i+1 : i;
    });
}
