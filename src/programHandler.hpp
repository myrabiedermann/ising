#pragma once

/*
 * program handler
 */

#include "global.hpp"
#include "utility/randomnumbers.hpp"
#include "MC.hpp"
#include "WLmethods/WL.hpp"
#include "WLmethods/WL-IT.hpp"
#include "MSM/msm.hpp"


struct programHandler
{

  void execute()
  {
      if( prms::MC )
      {
          std::cout << "\n--> performing a MC simulation ...\n";

          PATH outpath(prms::initialPath);
          outpath /= prms::output_key+".trj";
          mc MC;
          MC.do_metropolis(prms::mc_steps, prms::mc_printfreq);
          if( prms::SAVETRJ ) MC.save_trj(outpath);

          std::cout << "   ... done\n";
      }
      else if( prms::WL )
      {
          std::cout << "\n--> performing a WL simulation ...\n";

          wl WL;
          WL.do_simulation();
          WL.normalise_data();
          WL.save_data();

          std::cout << "   ... done\n";
      }
      else if( prms::IT )
      {
          std::cout << "\n--> performing an Inverse Time WL simulation ...\n";

          wl_it IT;
          IT.do_simulation();
          IT.normalise_data();
          IT.save_data();

          std::cout << "    ... done\n";
      }
      else if( prms::MSM )
      {
          std::cout << "\n--> performing a MSM simulation ...\n";

          PATH filepath(prms::initialPath);
          filepath /= prms::output_key+".data";
          msm MSM;
          if( prms::msm_READDATA )
          {
              PATH dirpath(prms::initialPath);
              dirpath /= prms::msm_read_directorypath;
              MSM.read_data(dirpath);
          }
          else MSM.create_data();
          MSM.extract_countMatrix();
          MSM.estimate_transitionMatrix();
          MSM.calc_entropies();
          MSM.save_data(filepath);

          if( prms::msm_ITS )
          {
              PATH itspath = filepath.replace_extension("");
              itspath += "-its.data";
              MSM.do_ITS(filepath);
          }
          if( prms::msm_CK )
          {
              PATH ckpath = filepath.replace_extension("");
              ckpath += "-ck.data";
              MSM.do_CK(ckpath);
          }
          if( prms::msm_EV )
          {
              PATH evpath = filepath.replace_extension("");
              evpath += "-ev.data";
              MSM.do_EV(evpath);
          }

          std::cout << "   ... MSM done\n";
      }
  }

};
