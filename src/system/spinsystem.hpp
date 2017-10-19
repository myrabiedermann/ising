#pragma once

#include "lib/enhance.hpp"
#include "spin.hpp"
#include "gui/parameters_widget.hpp"
#include <cassert>
#include <numeric>
#include <ostream>
#include <string>
#include <sstream>
#include <functional>

#include <iostream>


class Spinsystem
{
    float Hamiltonian = 0;
    std::vector<Spin>  spins {};
    std::vector<std::reference_wrapper<Spin>> lastFlipped {};
    ParametersWidget* parameters = Q_NULLPTR;

    float local_energy(const Spin&) const;
    float distance(const Spin&, const Spin&) const;
    float Jij(const SPINTYPE, const SPINTYPE) const;

public:
    Spinsystem()  {};
    ~Spinsystem() {};
    Spinsystem(const Spinsystem&) = delete;
    void operator=(const Spinsystem&) = delete;

    void setParameters(ParametersWidget*);
    void setup();

    void flip();
    void flip_back();

    inline unsigned long getWidth()  const { return parameters->getWidth(); }
    inline unsigned long getHeight() const { return parameters->getHeight(); }

    inline const decltype(spins)& getSpins() const { return spins; };
    inline auto getHamiltonian() const { return Hamiltonian; }
    
    template<SPINTYPE T> constexpr int num() const;

    void print(std::ostream & ) const;
    std::string str() const;
    const char* c_str() const;
};



template<SPINTYPE T>
constexpr inline int Spinsystem::num() const
{
    // return number of spins of type T
    return std::accumulate(std::begin(spins), std::end(spins), 0, [](int i, const Spin& S)
    {
        return S.get_type() == T ? i+1 : i;
    });
}
