#include "parameters.hpp"

/*
 * class prms
 */

// general Options:
std::string  prms::output_key;
bool         prms::SAVETRJ  = false;
// analysis Options:
bool         prms::MC = false;
bool         prms::WL = false;
bool         prms::IT = false;
bool         prms::MSM = false;
// system parameters
SIZE         prms::systemsize;
REAL         prms::J;
bool         prms::CONSTRAINED = false;
// MC parameters:
SIZE         prms::mc_steps;
SIZE         prms::mc_printfreq;
bool         prms::mc_SAVEPNG = false;
// Wang Landau parameters:
SIZE         prms::wl_checkflatness;
REAL         prms::wl_flatness;
REAL         prms::wl_convcrit;
// Inverse time parameters:
REAL         prms::it_convcrit;
SIZE         prms::it_checkOccupancy;
// MSM parameters:
bool         prms::msm_READDATA;
std::string  prms::msm_read_directorypath;
SIZE         prms::msm_maxMCsteps;
SIZE         prms::msm_runs;
SIZE         prms::msm_scaling;
SIZE         prms::msm_tau;
bool         prms::msm_ITS;
bool         prms::msm_CK;
bool         prms::msm_EV;
std::vector<std::pair<SIGNED, SIGNED>>  prms::msm_cktransitions;

boost::filesystem::path prms::initialPath;

// constructor:
prms::prms(int argc, char* argv[])
 : optionsMap( program_options (argc, argv) )
{
    initialPath = boost::filesystem::initial_path();
    // save all general program options:
    output_key = optionsMap["output"].as<decltype(output_key)>();
    if( optionsMap.count("savetrj") )     SAVETRJ = true;

    // save all analysis options:
    MC  = optionsMap.count("MC") ? true : false;
    WL  = optionsMap.count("WL") ? true : false;
    IT  = optionsMap.count("IT") ? true : false;
    MSM = optionsMap.count("MSM") ? true : false;

    // save system information:
    systemsize = optionsMap["size"].as<decltype(systemsize)>();
    J          = optionsMap["J"].as<decltype(J)>();
    CONSTRAINED= optionsMap.count("constrained") ? true : false;

    // save additional info:
    if( MC ){
        mc_steps       = optionsMap["MCsteps"].as<decltype(mc_steps)>();
        mc_printfreq   = optionsMap["printfreq"].as<decltype(mc_printfreq)>();
        mc_SAVEPNG     = optionsMap.count("savepng") ? true : false;
    }
    if( WL ){
        wl_checkflatness  = optionsMap["checkfreq"].as<decltype(wl_checkflatness)>();
        wl_flatness       = optionsMap["flatness"].as<decltype(wl_flatness)>();
        wl_convcrit       = optionsMap["convergence"].as<decltype(wl_convcrit)>();
    }
    if( IT ){
        it_checkOccupancy = optionsMap["checkfreqIT"].as<decltype(it_checkOccupancy)>();
        it_convcrit       = optionsMap["convergenceIT"].as<decltype(it_convcrit)>();
    }
    if( MSM ){
        msm_READDATA    = optionsMap.count("readdata") ? true : false;
        msm_read_directorypath = optionsMap["directorypath"].as<decltype(msm_read_directorypath)>();
        msm_runs        = optionsMap["MCruns"].as<decltype(msm_runs)>();
        msm_maxMCsteps  = optionsMap["maxMCsteps"].as<decltype(msm_maxMCsteps)>();
        msm_scaling     = optionsMap["scaling"].as<decltype(msm_scaling)>();
        msm_tau         = optionsMap["tau"].as<decltype(msm_tau)>();
        msm_ITS         = optionsMap.count("its")    ? true : false;
        msm_CK          = optionsMap.count("cktest") ? true : false;
        msm_EV          = optionsMap.count("ev")     ? true : false;

      if( optionsMap.count("cktransitions") )
      {
          std::string transitions = optionsMap["cktransitions"].as<decltype(transitions)>();
          // tokenize cktransitions to get transition pairs in format pair<int, int>
          typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
          boost::char_separator<char> separatorList("[],+ _;:","");  // first " ": dropped delimiters, second " ": kept delimiters
          boost::char_separator<char> separatorList2("-","");
          tokenizer tokens(transitions, separatorList);
          for(auto& pair: tokens)
          {
              tokenizer tokens2(pair, separatorList2);
              if( std::distance(tokens2.begin(), tokens2.end()) != 2) throw std::range_error("invalid number of tokens for ck transitions in __func__");
              tokenizer::iterator iter = tokens2.begin();
              int first = std::stoi(*iter);
              ++iter;
              int second = std::stoi(*iter);
              msm_cktransitions.emplace_back( first, second );
          }
      }

    }

    if( ! MC and ! WL and ! IT and ! MSM ){
        std::cout << "no simulation setup was provided ... ";
        std::cout << "not doing anything --> exit program\n";
        std::exit(0);
    }
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
    // analysis options:
    std::cout << "analysis options:\n";
    std::cout << formatted("MC", MC);
    std::cout << formatted("WL", WL);
    std::cout << formatted("IT", IT);
    std::cout << formatted("MSM", MSM);
    if(MC){
        std::cout << "MC options:\n";
        std::cout << formatted("MC steps", mc_steps);
        std::cout << formatted("print frequency", mc_printfreq);
        std::cout << formatted("Saving pngs", mc_SAVEPNG);
    }else if(WL){
        std::cout << "WL options:\n";
        std::cout << formatted("checkfreq:\t\t", wl_checkflatness);
        std::cout << formatted("   flatness:", wl_flatness);
        std::cout << formatted("convergence", wl_convcrit);
    }else if(IT){
        std::cout << "IT options:\n";
        std::cout << formatted("checkfreq:", it_checkOccupancy);
        std::cout << formatted("convergence:", it_convcrit);
    }else if(MSM){
        std::cout << "MSM options:\n";
        std::cout << formatted("read data", msm_READDATA);
        if(msm_READDATA) std::cout << formatted("directory path", msm_read_directorypath);
        std::cout << formatted("# runs", msm_runs);
        std::cout << formatted("max steps per run", msm_maxMCsteps);
        std::cout << formatted("scaling", msm_scaling);
        std::cout << formatted("tau", msm_tau);
        std::cout << formatted("ITS", msm_ITS);
        std::cout << formatted("CK test", msm_CK);
        if( msm_CK ) std::cout << formatted("CK transitions", msm_cktransitions);
        std::cout << formatted("Evalues/Evectors", msm_EV);
    }
}

/***************************************************************************/
