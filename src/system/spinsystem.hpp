#pragma once

#include "spin.hpp"
#include "lib/enhance.hpp"
#include "utility/histogram.hpp"
#include "utility/logger.hpp"
#include "gui/parameters/base_parameters_widget.hpp"
#include <ostream>
#include <string>
#include <sstream>



class Spinsystem
{
    BaseParametersWidget* parameters = Q_NULLPTR;
    double Hamiltonian = 0;
    std::vector<Spin> spins {};
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

    double             getMagnetisation() const;
    inline auto        getHamiltonian() const { return Hamiltonian; }
    inline const auto& getSpins() const { return spins; };

    inline unsigned long getHeight() const { Q_CHECK_PTR(parameters); return parameters->getHeight(); }
    inline unsigned long getWidth()  const { Q_CHECK_PTR(parameters); return parameters->getWidth(); }
    inline double        getInteraction() const { Q_CHECK_PTR(parameters); return parameters->getInteraction(); }
    inline double        getMagnetic() const { Q_CHECK_PTR(parameters); return parameters->getMagnetic(); }
    inline bool          getConstrained() const { Q_CHECK_PTR(parameters); return parameters->getConstrained(); }
    inline double        getRatio() const { Q_CHECK_PTR(parameters); return parameters->getRatio(); }
    inline bool          getWavelengthPattern() const { Q_CHECK_PTR(parameters); return parameters->getWavelengthPattern(); }
    inline int           getWavelength() const { Q_CHECK_PTR(parameters); return parameters->getWavelength(); }

    Histogram<double> computeCorrelation() const;
    Histogram<double> computeStructureFunction(const Histogram<double>) const;

    void print(std::ostream & ) const;
    std::string str() const;
};



