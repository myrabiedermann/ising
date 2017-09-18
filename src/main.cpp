#include "gui/mainwindow.hpp"
#include "lib/enhance.hpp"
#include "system/montecarlohost.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    #ifndef NDEBUG
    feenableexcept(FE_DIVBYZERO);
    feenableexcept(FE_INVALID);
    feenableexcept(FE_OVERFLOW);
    feenableexcept(FE_UNDERFLOW);
    #endif
    
//     enhance::seed = 123456789;
    enhance::seed = std::time(nullptr);
    enhance::rand_engine.seed(enhance::seed);
    qInfo() << "seed for random number generator:    " << enhance::seed;
    
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    
//     // save program options in "prms":
//     prms params(argc, argv);
//     
//     
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

