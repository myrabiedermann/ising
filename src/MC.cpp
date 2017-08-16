#include "MC.hpp"

/*
 * functions in class "mc"
 */

 // constructor:
 mc::mc()
  : spinsystem(prms::systemsize, prms::J, prms::CONSTRAINED)
{

    HwithoutJ_trj.push_back(spinsystem.get_HwithoutJ());

// debugging:
#ifndef NDEBUG
    std::cout << "initial: H= " << spinsystem.get_Hamiltonian() << "\n";
    std::cout << "         H/J = " << spinsystem.get_HwithoutJ() << "\n";
    std::cout << spinsystem;
#endif

}

/***************************************************************************/

void mc::do_metropolis(const unsigned int& _steps, const unsigned int& _printfreq)
{

    REAL energy_before;
    REAL energy_after;

    print_frequency = _printfreq;

    // create folder for pngs if necessary
    std::string dir_path_string = "";
    if( prms::mc_SAVEPNG ){
        PATH dir_path(prms::initialPath);
        dir_path /= prms::output_key+"-png";
        dir_path_string = prms::output_key+"-png";
        if( boost::filesystem::exists(dir_path) and boost::filesystem::is_directory(dir_path) )
        {
            boost::filesystem::remove_all(dir_path);
#ifndef NDEBUG
            std::cout << "WARNING: removed all contents in trajectory " << dir_path << "\n";
#endif
        }
        boost::filesystem::create_directory(dir_path);
    }


    for(SIZE t=1; t<=_steps; ++t)
    {
        // flip spin:
        energy_before = spinsystem.get_Hamiltonian();
        spinsystem.flip();
        energy_after = spinsystem.get_Hamiltonian();
        // check metropolis criterion:
        auto rd = randomDouble(0.0, 1.0);
        if( rd >= std::exp(-(energy_after-energy_before)) ){
            spinsystem.flip_back(); // flip back if move not accepted
#ifndef NDEBUG
            std::cout << "random = " << rd
                    << " >= " << std::exp(-(energy_after-energy_before))
                    << " = exp(-(energy_after-energy_before))" << "\n";
            std::cout << "new H would have been: " << energy_after << "\n";
        }
        else{
            std::cout << rd << " < " << std::exp(-(energy_after-energy_before)) << "\n";
            std::cout << "new H: " << energy_after << "\n" << spinsystem;
#endif
        }

        // save energy in HwithoutJ_trj:
        if( t % _printfreq == 0 ) {
            HwithoutJ_trj.push_back(spinsystem.get_HwithoutJ());
            // save snapshots to png:
            if( prms::mc_SAVEPNG ){
                pngmaker PNG(60);
                std::stringstream snapshot;
                snapshot << spinsystem;
                PNG.save(dir_path_string, t, snapshot);
            }
        }
    }

}

/***************************************************************************/

void mc::save_trj(const boost::filesystem::path& _filepath)
{
  // save trajectory to file _filepath

  OFSTREAM OUT(_filepath);
  // header line:
  OUT << std::setw(10) << "# time" << std::setw(6) << "H / J, J = " << prms::J << "\n";
  // data lines:
  for(SIZE t=0; t<HwithoutJ_trj.size(); ++t){
      OUT << std::setw(10) << t*print_frequency
          << std::setw(6)  << HwithoutJ_trj[t] << "\n";
  }
  OUT.close();
}
