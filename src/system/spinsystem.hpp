#pragma once

#include "lib/enhance.hpp"
#include "spin.hpp"
#include "histogram.hpp"
#include "gui/default_parameters_widget.hpp"
#include <cassert>
#include <numeric>
#include <ostream>
#include <string>
#include <sstream>
#include <functional>

#include "utility/logger.hpp"


class Spinsystem
{
    double Hamiltonian {0};
    std::vector<Spin>  spins {};
    std::vector<std::reference_wrapper<Spin>> lastFlipped {};
    BaseParametersWidget* parameters = Q_NULLPTR;

    void computeHamiltonian();
    double local_energy_interaction(const Spin&) const;
    double local_energy_magnetic(const Spin&) const;
    double Jij(const SPINTYPE, const SPINTYPE) const;
    double Bi(const SPINTYPE) const;
    double distance(const Spin&, const Spin&) const;

public:
    Spinsystem()  {};
    ~Spinsystem() {};
    Spinsystem(const Spinsystem&) = delete;
    void operator=(const Spinsystem&) = delete;

    void setParameters(BaseParametersWidget*);
    void setup();
    void resetParameters();
    void resetSpins();
    void resetSpinsCosinus(const double);

    void flip();
    void flip_back();

    Histogram<double> getCorrelation() const;
    double getMagnetisation() const;

    std::vector<double> computeAmplitudes();

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


