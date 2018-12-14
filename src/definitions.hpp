/*
 * some useful global definitions
 */

#pragma once


#include <iostream>

#define isingLOG(x) {std::clog << "[LOG] "; do { std::clog << x; } while (0); std::clog << '\n';}

#ifndef NDEBUG
    #define isingDEBUG(x) {std::cerr << "[DEBUG] "; do { std::cerr << x; } while (0); std::cerr << '\n';}
#else
    #define isingDEBUG(x)
#endif
