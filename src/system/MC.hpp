#pragma once

#include "global.hpp"
#include "utility/randomnumbers.hpp"
// #include "input/parameters.hpp"
#include "gui/parameters_widget.hpp"
#include "system/spinsystem.hpp"


/*
 * class mc
 */


class mc {
    
private:
    ParametersWidget* parameters = Q_NULLPTR;
    Spinsystem spinsystem {};
    std::vector<REAL> trajectory {};


public:
    mc();
    mc(const mc&) = delete;
    void operator=(const mc&) = delete;
    ~mc();
    
    void setup();
    
    void setParameters(ParametersWidget*);

    void  do_metropolis(const unsigned long&);

    void  save_trj(const PATH&);
    
    
    const auto& getSpinsystem() const { return spinsystem; }
    
    const auto& get_trajectory() const { return trajectory; }
    auto&       get_trajectory()       { return trajectory; }
};
