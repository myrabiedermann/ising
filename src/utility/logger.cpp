#include "logger.hpp"



    
Logger::Logger() 
  : logfile("/dev/null", std::ios::app | std::ios::out)
{
    if ( !ProgramOptions::getInstance()("nolog"))
    {
        if(logfile.is_open())
            logfile.close();
        logfile.open("pispector.log", std::ios::app | std::ios::out);
    }
}




Logger::~Logger()
{
    logfile << SYMBOL::get<SEPERATOR::NEWLINE>();
    
    if (logfile.is_open())
        logfile.close();
}




std::_Put_time<char> Logger::wallTime()
{
    using namespace std::chrono;
    std::time_t now = system_clock::to_time_t( system_clock::now() );
    return std::put_time(std::localtime(&now), "%F %T");
}