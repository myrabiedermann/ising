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
		("help,h",                                                                  "produce help message")
		("config",   po::value<std::string>(&config_file)->default_value("CONFIG"), "specify a config file")
        ("output,o", po::value<std::string>()->default_value("output"),             "specify an output key")
        ("savetrj",                                                                 "save the generated trajectory");

    po::options_description systemOptions("System options");
    systemOptions.add_options()
        ("size",        po::value<SIZE>()->default_value(6),    "system size")
        ("J",           po::value<REAL>()->default_value(1.0),  "interaction strength parameter J")
        ("constrained",                                         "constrain to equal numbers of up/down spins");

    po::options_description mcOptions("MC options");
    mcOptions.add_options()
        ("MCsteps",    po::value<SIZE>()->default_value(10), "# of MC steps")
        ("printfreq",  po::value<SIZE>()->default_value(1),  "printing frequency");


	po::options_description allOptions("");
	allOptions.add(generalOptions).add(systemOptions).add(mcOptions);

	// create an optionsMap:
	po::variables_map optionsMap;

	try
	{
		// ... and parse all options from command line to that optionsMap:
        po::store( po::command_line_parser(argc, argv).options(allOptions).run(), optionsMap);
		po::notify( optionsMap );

		// .. now parse all options from config file:
		std::ifstream FILE(config_file);
		if( !FILE ){
			std::cout << "--> did not find config file, using options from command line\n";
		}else{
			po::store( po::parse_config_file(FILE, allOptions), optionsMap) ;
			po::notify( optionsMap );
		}
		FILE.close();
	} 
	catch(const std::exception& e)
    {
		std::cout << "ERROR in parsing program options:\n";
        std::cout << e.what() << "\n";
        std::exit(EXIT_FAILURE);
	}


	// if there was a call for help:
	if( optionsMap.count("help") )
    {
        std::cout << generalOptions;;
        std::cout << systemOptions;
        std::cout << mcOptions;
        std::exit(EXIT_SUCCESS);
	}

	
	return optionsMap;
}
