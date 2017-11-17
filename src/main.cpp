#include "gui/mainwindow.hpp"
#include "lib/enhance.hpp"
#include "system/montecarlohost.hpp"
#include <QApplication>
#include "utility/logger.hpp"



int main(int argc, char *argv[])
{
    Logger::getInstance().write_new_line( "[GENERAL]" , "ISING LOG FILE");

    enhance::seed = std::time(nullptr);
    #ifndef NDEBUG
    enhance::seed = 123456789;
    #endif
    enhance::rand_engine.seed(enhance::seed);
    Logger::getInstance().write_new_line("[GENERAL]", "seed for random number generator:", enhance::seed) ;

    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();

}

