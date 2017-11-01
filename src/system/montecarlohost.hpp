#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "gui/parameters_widget.hpp"
#include "spinsystem.hpp"
#include "histogram.hpp"
#include "acceptance.hpp"
#include "lib/enhance.hpp"
#include <QDebug>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <fstream>



class MonteCarloHost
{
    ParametersWidget* parameters = Q_NULLPTR;
    AcceptanceAdaptor* acceptance = nullptr;
    Spinsystem           spinsystem {};

    std::vector<double>  trajectory {};
    std::vector<double>  magnetisations {};
    std::vector<double>  susceptibilities {};
    std::vector<histogram<double>> correlations {};
    
protected:
    
public:
    MonteCarloHost();
    MonteCarloHost(const MonteCarloHost&) = delete;
    void operator=(const MonteCarloHost&) = delete;
    ~MonteCarloHost();
    
    void setup();
    void setParameters(ParametersWidget*);
    void setAcceptance(AcceptanceAdaptor*);
    
    const Spinsystem&               getSpinsystem() const;
    const decltype(trajectory)&     getTrajectory() const;
    const decltype(correlations)&   getCorrelations() const;
    
    void run(const unsigned long&, const bool& EQUIL = false);
    
    void print_trajectory();
    void print_correlation();
    void print_data();
};
