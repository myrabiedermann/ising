#pragma once

#include "lib/enhance.hpp"
#include "spin.hpp"
#include "histogram.hpp"
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
    double Hamiltonian {0};
    std::vector<Spin>  spins {};
    std::vector<std::reference_wrapper<Spin>> lastFlipped {};
    ParametersWidget* parameters = Q_NULLPTR;

    void computeHamiltonian();
    double calcHamiltonian();
    double local_energy_interaction(const Spin&) const;
    double local_energy_magnetic(const Spin&) const;
    double Jij(const SPINTYPE, const SPINTYPE) const;
    double Bi(const SPINTYPE) const;
    double distance(const Spin&, const Spin&) const;

    void setSpins();

public:
    Spinsystem()  {};
    ~Spinsystem() {};
    Spinsystem(const Spinsystem&) = delete;
    void operator=(const Spinsystem&) = delete;

    void setParameters(ParametersWidget*);
    void setup();
    void resetParameters();
    void resetSpins();

    void flip();
    void flip_back();

    histogram<double> getCorrelation() const;
    inline double getMagnetisation() const;
    inline double getMagnetisationSquared() const;
    inline double getSusceptibility() const;

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


inline double Spinsystem::getMagnetisation() const
{
    // return average magnetisation: <M> = 1/N sum( S_i )
    return ( (double) num<SPINTYPE::UP>() - num<SPINTYPE::DOWN>() ) / spins.size();
}


inline double Spinsystem::getMagnetisationSquared() const
{
    // return average magnetisation squared: <M^2> = 1/N sum( S_i*S_i )
    return ( (double) num<SPINTYPE::UP>() + num<SPINTYPE::DOWN>() ) / spins.size();
}


inline double Spinsystem::getSusceptibility() const
{
    // return susceptibility: chi = dM/dB = ( <M^2> - <M>^2 ) / ( k_b * T )
    return ( getMagnetisationSquared() - getMagnetisation()*getMagnetisation() ) / parameters->getTemperature();
}
