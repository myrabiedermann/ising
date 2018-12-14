
#include "gui/mainwindow.hpp"
#include "lib/enhance.hpp"
#include "definitions.hpp"

#include <QApplication>
#include <random>



int main(int argc, char *argv[])
{

    isingLOG( " - - - ising program - - - " )

    enhance::seed = std::random_device{}();
    #ifndef NDEBUG
        enhance::seed = 123456789;
    #endif
    enhance::rand_engine.seed(enhance::seed);
    isingLOG("main: " << "seed for random number generator: " << enhance::seed)


    QApplication app(argc, argv);
    MainWindow w;
    w.show();


    return app.exec();

}

