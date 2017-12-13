#include "gui/mainwindow.hpp"
#include "lib/enhance.hpp"
#include "utility/logger.hpp"
#include <QApplication>
#include <random>



int main(int argc, char *argv[])
{

    Logger::getInstance().write( "ISING LOG FILE");


    enhance::seed = std::random_device{}();
    #ifndef NDEBUG
        enhance::seed = 123456789;
    #endif
    enhance::rand_engine.seed(enhance::seed);
    Logger::getInstance().write_new_line("[main]", "seed for random number generator:", enhance::seed) ;


    QApplication app(argc, argv);
    MainWindow w;
    w.show();


    return app.exec();
}

