#pragma once

/*
 *  program options
 */

#include "../global.hpp"


inline boost::program_options::variables_map program_options (int argc, char* argv[])
{
	namespace po = boost::program_options;

	std::string config_file;

	// set possible options:
	po::options_description generalOptions("General options");
	generalOptions.add_options()
		("help,h",   "produce help message")
		("config",    po::value<std::string>(&config_file)->default_value("CONFIG"),  "specify a config file")
    ("output,o",  po::value<std::string>()->default_value("output"),  "specify an output key")
    ("savetrj",  "save all generated trajectories (only: MC & MSM) ");

	po::options_description analysisOptions("Analysis options");
	analysisOptions.add_options()
		("MC", "perform a MC simulation")
		("WL", "perform a WL simulation")
		("IT", "perform a 1/t WL simulation")
		("MSM", "perform a MSM simulation");

  po::options_description systemOptions("System options");
  systemOptions.add_options()
    ("size",				 po::value<SIZE>()->default_value(6),      "system size")
    ("J",    				 po::value<REAL>()->default_value(1.0),  "interaction strength parameter J")
		("constrained",  "constrain to equal numbers of up/down spins");

  po::options_description mcOptions("MC options");
  mcOptions.add_options()
    ("MCsteps",    po::value<SIZE>()->default_value(10), "# of MC steps")
    ("printfreq",  po::value<SIZE>()->default_value(1),  "printing frequency")
    ("savepng",		 "save snapshots as .png");

  po::options_description wlOptions("WL options");
  wlOptions.add_options()
    ("flatness",    po::value<REAL>()->default_value(0.8), 			  "flatness criterion ( < 1.0 )")
    ("checkfreq",   po::value<SIZE>()->default_value(1000), "frequency for checking flatness")
    ("convergence", po::value<REAL>()->default_value(1e-8), 			"convergence criterion");

  po::options_description itOptions("Inversetime options");
	itOptions.add_options()
    ("checkfreqIT",   po::value<SIZE>()->default_value(1000), "frequency for checking flatness")
    ("convergenceIT", po::value<REAL>()->default_value(1e-8), 			"convergence criterion");

	po::options_description msmOptions("MSM options");
	msmOptions.add_options()
		("readdata",   "read MC data from a files")
		("directorypath",  po::value<std::string>()->default_value("trjs"),"directory path from which to read MC data")
		("MCruns", 		 po::value<SIZE>()->default_value(1000), "# of MC runs")
		("maxMCsteps", po::value<SIZE>()->default_value(1e6),  "max # of MC steps per run")
		("scaling", 	 po::value<SIZE>()->default_value(1), 	 "scaling of long MC runs to short MC runs")
		("tau", 			 po::value<SIZE>()->default_value(1), 	 "lag time")
		("its", 			 "compute implied time scales")
		("cktest", 		 "perform a Chapman Kolmogorov test")
		("cktransitions", po::value<std::string>(), "transitions to check via CK test")
		("ev",				 "compute the eigenvalues & eigenvectors of the model");

	po::options_description allOptions("");
	allOptions.add(generalOptions).add(analysisOptions).add(systemOptions).add(mcOptions).add(wlOptions).add(itOptions).add(msmOptions);

	// create an optionsMap:
	po::variables_map optionsMap;

	try{
		// ... and parse all options from command line to that optionsMap:
	  po::store( po::command_line_parser(argc, argv).options(allOptions).run(), optionsMap);
		po::notify( optionsMap );

		// .. now parse all options from config file:
		std::ifstream FILE(config_file);
		if( !FILE ){
			std::cout << "--> did not find config file, using only options from command line\n";
		}else{
			po::store( po::parse_config_file(FILE, allOptions), optionsMap) ;
			po::notify( optionsMap );
		}
		FILE.close();
	} catch(const std::exception& e){
			std::cout << "ERROR in parsing program options:\n";
      std::cout << e.what() << "\n";
      std::exit(EXIT_FAILURE);
	}


	// if there was a call for help:
	if( optionsMap.count("help") ){
			std::cout << generalOptions;
      std::cout << analysisOptions;
      std::cout << systemOptions;
      if( optionsMap.count("MC") )  std::cout << mcOptions;
      if( optionsMap.count("WL") )  std::cout << wlOptions;
      if( optionsMap.count("IT") )  std::cout << itOptions;
			if( optionsMap.count("MSM") ) std::cout << msmOptions;
			std::exit(EXIT_SUCCESS);
	}

	return optionsMap;
}
