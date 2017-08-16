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
// analysis Options:
  static bool         MC;
  static bool         WL;
  static bool         IT;
  static bool         MSM;
// system parameters
  static SIZE         systemsize;
  static REAL      J;
  static bool         CONSTRAINED;
// MC parameters:
  static SIZE         mc_steps;
  static SIZE         mc_printfreq;
  static bool         mc_SAVEPNG;
// Wang Landau parameters:
  static SIZE         wl_checkflatness;
  static REAL         wl_flatness;
  static REAL         wl_convcrit;
// Inverse time parameters:
  static REAL         it_convcrit;
  static SIZE         it_checkOccupancy;
// MSM parameters:
  static bool         msm_READDATA;
  static std::string  msm_read_directorypath;
  static SIZE         msm_maxMCsteps;
  static SIZE         msm_runs;
  static SIZE         msm_scaling;
  static SIZE         msm_tau;
  static bool         msm_ITS;
  static bool         msm_CK;
  static bool         msm_EV;
  static std::vector<std::pair<SIGNED, SIGNED>>  msm_cktransitions;

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


// specialisation of template function prms::formatted(...)
template <>
inline std::string prms::formatted<>(const std::string& S, const std::vector<std::pair<int, int>>& vars)
{
  std::ostringstream STREAM;
  STREAM << std::boolalpha << std::left;
  STREAM << "   ";
  STREAM << std::setw(30) << S;
  for(auto& var: vars)
  {
      STREAM << std::setw(4) << std::right << var.first;
      STREAM << "-";
      STREAM << std::setw(4) << std::left << var.second;
  }
  STREAM << "\n";
  return STREAM.str();
}
