#include "parameters.hpp"

/*
 * class prms
 */

// general Options:
std::string  prms::output_key;
bool         prms::SAVETRJ  = false;
// system parameters
SIZE         prms::systemsize;
REAL         prms::J;
bool         prms::CONSTRAINED = false;
// MC parameters:
SIZE         prms::mc_steps;
SIZE         prms::mc_printfreq;

PATH         prms::initialPath;

// constructor:
prms::prms(int argc, char* argv[])
 : optionsMap( program_options (argc, argv) )
{
    initialPath = boost::filesystem::initial_path();
    
    // save all general program options:
    output_key = optionsMap["output"].as<decltype(output_key)>();
    SAVETRJ    = optionsMap.count("savetrj") ? true : false;

    // save system information:
    systemsize = optionsMap["size"].as<decltype(systemsize)>();
    J          = optionsMap["J"].as<decltype(J)>();
    CONSTRAINED= optionsMap.count("constrained") ? true : false;

    // save MC options:
    mc_steps     = optionsMap["MCsteps"].as<decltype(mc_steps)>();
    mc_printfreq = optionsMap["printfreq"].as<decltype(mc_printfreq)>();


    // print program options:
    print_flags();
}

/***************************************************************************/

void prms::print_flags()
{
    // general options:
    std::cout << "--> working with the following options:\n";
    std::cout << "general options:\n";
    std::cout << formatted("output key", output_key);
    std::cout << formatted("saving trajectories", SAVETRJ);
    // system options:
    std::cout << "system options:\n";
    std::cout << formatted("system size", systemsize);
    std::cout << formatted("J", J);
    std::cout << formatted("constrained system", CONSTRAINED);
    // mc options:
    std::cout << "MC options:\n";
    std::cout << formatted("MC steps", mc_steps);
    std::cout << formatted("print frequency", mc_printfreq);
}

/***************************************************************************/
