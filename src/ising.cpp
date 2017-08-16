/*
 * program 2D Ising
 *
 * ... performs simulations of the
 * standard Ising model in 2 dimensions & the constrained version (#spin-up = #spin-down):
 *
 * - Metropolis Monte Carlo
 * - Wang-Landau
 * - 1/t Wang-Landau
 * - MSM of MC data
 *
 * author: Myra Biedermann
 */

#include "global.hpp"
#include "programHandler.hpp"
#include "utility/randomnumbers.hpp"
#include "input/parameters.hpp"



int main(int argc, char* argv[])
{

// #ifndef NDEBUG
//   feenableexcept(FE_DIVBYZERO);
//   feenableexcept(FE_INVALID);
//   feenableexcept(FE_OVERFLOW);
//   feenableexcept(FE_UNDERFLOW);
// #endif

    // save program options in "prms":
    prms params(argc, argv);

    // set seed for random number generator:
    // seed = std::time(nullptr);
    seed = 123456789;
    std::cout << "--> seed for random number generator: \n   " << seed << "\n";

    // run simulations:
    programHandler handler;
    handler.execute();


    return 0;
}
