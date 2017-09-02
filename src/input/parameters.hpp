#pragma once

/*
 * class prms
 */

#include "../global.hpp"
#include "program_options.hpp"


class prms
{
private:
    const boost::program_options::variables_map optionsMap;
    void print_flags();

    template <typename T>
    std::string formatted(const std::string&, const T&);

public:
    prms(int argc, char* argv[]);
    // general Options:
    static std::string  output_key;
    static bool         SAVETRJ;
    // system parameters
    static SIZE         systemsize;
    static REAL         J;
    static bool         CONSTRAINED;
    // MC parameters:
    static SIZE         mc_steps;
    static SIZE         mc_printfreq;

    // additional stuff:
    static boost::filesystem::path initialPath;

};

template <typename T>
inline std::string prms::formatted(const std::string& S, const T& var)
{
    std::ostringstream STREAM;
    STREAM << std::boolalpha << std::left;
    STREAM << "   ";
    STREAM << std::setw(30) << S;
    STREAM << std::setw(15) << var;
    STREAM << "\n";
    return STREAM.str();
}

