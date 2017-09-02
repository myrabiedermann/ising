#include "MC.hpp"

/*
 * functions in class "mc"
 */


 mc::mc()
  : spinsystem(prms::systemsize, prms::J, prms::CONSTRAINED)
{

    trajectory.push_back(spinsystem.get_Hamiltonian());

#ifndef NDEBUG
    std::cout << "initial: H = " << spinsystem.get_Hamiltonian() << "\n";
    std::cout << spinsystem;
#endif

}

/***************************************************************************/

void mc::do_metropolis()
{

    REAL energy_before;
    REAL energy_after;


    for(SIZE t=1; t<=prms::mc_steps; ++t)
    {
        // flip spin:
        energy_before = spinsystem.get_Hamiltonian();
        spinsystem.flip();
        energy_after = spinsystem.get_Hamiltonian();
      
        // check metropolis criterion:
        auto rd = random_double(0.0, 1.0);
        if( rd >= std::exp(-(energy_after-energy_before)) ){
            spinsystem.flip_back(); // flip back if move not accepted
#ifndef NDEBUG
            std::cout << "random = " << rd
                      << " >= " << std::exp(-(energy_after-energy_before))
                      << " = exp(-(energy_after-energy_before))" << "\n";
            std::cout << "new H would have been: " << energy_after << "\n";
        }
        else
        {
            std::cout << rd << " < " << std::exp(-(energy_after-energy_before)) << "\n";
            std::cout << "new H: " << energy_after << "\n" << spinsystem;
#endif
        }
        
        // save to trajectory
        if( t % prms::mc_printfreq == 0 ) {
            trajectory.push_back(spinsystem.get_Hamiltonian());
        }
    }

}

/***************************************************************************/

void mc::save_trj(const PATH& _filepath)
{

    OFSTREAM OUT(_filepath);
    OUT << std::setw(10) << "# time" << std::setw(6) << "Hamiltonian\n";
    for(SIZE t=0; t<trajectory.size(); ++t){
        OUT << std::setw(10) << t*prms::mc_printfreq
            << std::setw(6)  << trajectory[t] << "\n";
    }
    OUT.close();
}
