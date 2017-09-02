#pragma once

/* *** *** *** *** *
 * GLOBAL INCLUDES *
 * *** *** *** *** */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cfenv>

#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <array>

#include <algorithm>
#include <functional> // std::reference_wrapper
#include <utility>    // std::pair
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <stdexcept>
#include <cassert>




/* *** *** *** *** *
 * GLOBAL TYPEDEFS *
 * *** *** *** *** */

typedef std::int16_t  SHORTSIGNED;
typedef std::int32_t  SIGNED;
typedef std::int64_t  LONGSIGNED;
typedef std::uint16_t SHORTSIZE;
typedef std::uint32_t SIZE;
typedef std::size_t   LONGSIZE;

typedef float SP;
typedef double DP;
typedef DP REAL;

typedef boost::filesystem::path     PATH;
typedef boost::filesystem::ofstream OFSTREAM;
typedef boost::filesystem::ifstream IFSTREAM;
