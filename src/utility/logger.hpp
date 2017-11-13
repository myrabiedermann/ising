// Copyright 2017 Simon Raschke

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
// limitations under the License.

#pragma once


#include "singleton.hpp"
#include <mutex>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <cassert>



enum class SEPERATOR : char 
{ 
    WHITESPACE = ' ', 
    COMMA = ',', 
    SEMICOLON = ';', 
    SLASH = '/', 
    BACKSLASH = '\\', 
    NONE = '\0' ,
    NEWLINE = '\n'
};



struct SYMBOL
{
    template<SEPERATOR sep>
    static constexpr inline char get() 
    {
        return static_cast<char>(sep);
    }
};



struct Logger
  : public Singleton<Logger>
{
    friend struct Singleton<Logger>;
    
    template<SEPERATOR sep = SEPERATOR::WHITESPACE, typename ... Args>
    void write_new_line( Args&& ... args ); 
    
    template<SEPERATOR sep = SEPERATOR::NONE, typename ... Args>
    void write( Args&& ... args ); 
    
    template<SEPERATOR sep = SEPERATOR::WHITESPACE, typename ... Args>
    void debug( Args&& ... args ); 
    
    // std::_Put_time<char> wallTime();
    
    
private:
    Logger();
    ~Logger();
    
    template <SEPERATOR sep, typename... Args>
    constexpr inline typename std::enable_if<sep!=SEPERATOR::NONE>::type write_args(Args&&... args);
    
    template <SEPERATOR sep, typename... Args>
    constexpr inline typename std::enable_if<sep==SEPERATOR::NONE>::type write_args(Args&&... args); 
    
    Logger(const Logger&) = delete;
    Logger& operator = (const Logger&) = delete;
    
    std::mutex mutex { };
    std::ofstream logfile;
    
};




template<SEPERATOR sep, typename ... Args>
void Logger::write_new_line( Args&& ... args ) 
{
    assert(&getInstance());
    assert(logfile.is_open());
    std::lock_guard<std::mutex> lock(mutex);
    logfile << SYMBOL::get<SEPERATOR::NEWLINE>();
    // logfile << wallTime();
    write_args<sep>(args...);
    logfile.flush();
}




template<SEPERATOR sep, typename ... Args>
void Logger::write( Args&& ... args ) 
{
    assert(&getInstance());
    assert(logfile.is_open());
    std::lock_guard<std::mutex> lock(mutex);
    write_args<sep>(args...);
    logfile.flush();
}



template<SEPERATOR sep, typename ... Args>
void Logger::debug( Args&& ... args ) 
{
    assert(&getInstance());
    assert(logfile.is_open());
    #ifndef NDEBUG
    std::lock_guard<std::mutex> lock(mutex);
    logfile << SYMBOL::get<SEPERATOR::NEWLINE>();
    // logfile << wallTime();
    write_args<sep>(args...);
    logfile.flush();
    #else
    return;
    #endif
}



template <SEPERATOR sep, typename... Args>
constexpr inline typename std::enable_if<sep!=SEPERATOR::NONE>::type Logger::write_args(Args&&... args)
{
    assert(&getInstance());
    assert(logfile.is_open());
    using expander = int[];
    (void) expander {0, (void(logfile << SYMBOL::get<sep>() << std::forward<Args>(args)),0)...};
}




template <SEPERATOR sep, typename... Args>
constexpr inline typename std::enable_if<sep==SEPERATOR::NONE>::type Logger::write_args(Args&&... args) 
{
    assert(&getInstance());
    assert(logfile.is_open());
    using expander = int[];
    (void) expander {0, (void(logfile << std::forward<Args>(args)),0)...};
}