#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "gui/parameters/base_parameters_widget.hpp"
#include "spinsystem.hpp"
#include "histogram.hpp"
#include "lib/enhance.hpp"
#include "definitions.hpp"
#include <QDebug>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <fstream>



class MonteCarloHost
{
private:
    Spinsystem           spinsystem {};
    std::vector<double>  energies {};
    std::vector<double>  magnetisations {};

    bool acceptance(const double, const double, const double);
    
public:
    void run(const unsigned long&, const bool EQUILMODE = false);


/* 
 * DIE FOLGENDEN FUNKTIONEN SOLLTEN NUETZLICH FUER DIE
 * IMPLEMENTIERUNGSAUFGABEN SEIN
 * IHRE KONKRETE IMPLEMENTIERUNG MUSS ZUR VERWENDUNG DER FUNKTIONEN 
 * JEDOCH NICHT BEKANNT SEIN
 */ 

public:
    double getTemperature() const;         // returns temperature



/*
 * DER HIER FOLGENDE TEIL DER KLASSE IST NICHT RELEVANT FUER 
 * DIE IMPLEMENTIERUNGSAUFGABEN UND KANN IGNORIERT WERDEN 
 */

private:
    BaseParametersWidget* parameters = Q_NULLPTR;

public:
    MonteCarloHost();
    MonteCarloHost(const MonteCarloHost&) = delete;
    void operator=(const MonteCarloHost&) = delete;
    ~MonteCarloHost();
    
    void setParameters(BaseParametersWidget*);
    void setup();
    void resetSpins();
    void clearRecords();
    
    const Spinsystem& getSpinsystem() const;
    
    void print_data() const;
    void print_averages() const;
    void print_correlation(Histogram<double>&) const;
    void print_structureFunction(Histogram<double>&) const;
};
