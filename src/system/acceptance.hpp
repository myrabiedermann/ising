#pragma once


#ifndef USE_MATH_DEFINES
    #define USE_MATH_DEFINES
#endif


#include "lib/enhance.hpp"
#include <cmath>


struct AcceptanceAdaptor
{
    virtual bool valid(double energy_old, double energy_new, double temperature = 1.0) = 0;
    virtual ~AcceptanceAdaptor(){};
    #ifndef QT_NO_DEBUG 
    virtual inline double latestConditionValue() const { return latestCondition; }
    virtual inline double latestRandomNumber() const { return latestRandom; }
    #endif
    
protected:
    AcceptanceAdaptor() {};
    
    #ifndef QT_NO_DEBUG
    double latestCondition = 0;
    double latestRandom = 0;
    #endif
};


struct Metropolis : public AcceptanceAdaptor
{    
    virtual bool valid(double energy_old, double energy_new, double temperature = 1.0) override 
    {
        #ifndef QT_NO_DEBUG
        latestRandom = enhance::random_double(0.0, 1.0);
        latestCondition = std::exp(-(energy_new-energy_old)/temperature);
        return latestRandom < latestCondition ? true : false;
        #endif
        return enhance::random_double(0.0, 1.0) < std::exp(-(energy_new-energy_old)/temperature) ? true : false;
    }
};
