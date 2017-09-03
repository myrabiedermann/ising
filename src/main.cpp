#include "gui/ising.hpp"
#include "global.hpp"
#include "utility/randomnumbers.hpp"
#include "input/parameters.hpp"
#include "MC.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    #ifndef NDEBUG
    feenableexcept(FE_DIVBYZERO);
    feenableexcept(FE_INVALID);
    feenableexcept(FE_OVERFLOW);
    feenableexcept(FE_UNDERFLOW);
    #endif
    
    QApplication app(argc, argv);
    ising w;
    w.show();

    
//     // save program options in "prms":
//     prms params(argc, argv);
//     
//     
//     // seed = std::time(nullptr);
//     seed = 123456789;
//     rand_engine.seed(seed);
//     std::cout << "seed for random number generator: \n   " << seed << "\n";
//     
//     // run simulation:
//     mc MC;
//     MC.do_metropolis();
//     if( prms::SAVETRJ )
//     {
//         PATH outpath(prms::initialPath);
//         outpath /= prms::output_key+".trj";
//         MC.save_trj(outpath);
//     }
    
    return app.exec();
}

