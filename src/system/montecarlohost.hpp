#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


// #include "global.hpp"
#include "gui/parameters_widget.hpp"
#include "spinsystem.hpp"
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
    std::vector<double> trajectory {};
    Spinsystem spinsystem {};
    
protected:
    
public:
    MonteCarloHost();
    MonteCarloHost(const MonteCarloHost&) = delete;
    void operator=(const MonteCarloHost&) = delete;
    ~MonteCarloHost();
    
    void setup();
    void setParameters(ParametersWidget*);
    void setAcceptance(AcceptanceAdaptor*);
    
    const Spinsystem& getSpinsystem() const;
    const decltype(trajectory)& getTrajectory() const;
    
    void run(const unsigned long&);
    
    void print_trajectory(std::ofstream&);
};
