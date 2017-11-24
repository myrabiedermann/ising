#pragma once

#include "lib/enhance.hpp"
#include "spin.hpp"
#include "utility/histogram.hpp"
#include "utility/logger.hpp"
#include "gui/default_parameters_widget.hpp"
#include <cassert>
#include <ostream>
#include <string>
#include <sstream>



class Spinsystem
{
    BaseParametersWidget* parameters = Q_NULLPTR;
    double Hamiltonian {0};
    std::vector<Spin>  spins {};
    std::vector<std::reference_wrapper<Spin>> lastFlipped {};

    void   computeHamiltonian();
    double local_energy_interaction(const Spin&) const;
    double local_energy_magnetic(const Spin&) const;
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

    double            getMagnetisation() const;
    inline auto       getHamiltonian() const { return Hamiltonian; }
    inline unsigned long getWidth()  const { return parameters->getWidth(); }
    inline unsigned long getHeight() const { return parameters->getHeight(); }
    inline const auto& getSpins() const { return spins; };

    Histogram<double>   computeCorrelation() const;
    std::vector<double> computeStructureFunction(const Histogram<double>) const;

    void print(std::ostream & ) const;
    std::string str() const;
};



