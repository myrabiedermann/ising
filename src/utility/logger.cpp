#include "logger.hpp"



    
Logger::Logger() 
  : logfile("ising.log", std::ios::out)
{}


Logger::~Logger()
{
    logfile << SYMBOL::get<SEPERATOR::NEWLINE>();
    
    if (logfile.is_open())
        logfile.close();
}

