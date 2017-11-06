#include "gui/mainwindow.hpp"
#include "lib/enhance.hpp"
#include "system/montecarlohost.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    
    enhance::seed = std::time(nullptr);
    enhance::seed = 123456789;
    enhance::rand_engine.seed(enhance::seed);
    qInfo() << "seed for random number generator:    " << enhance::seed;
    
    QApplication app(argc, argv);
    qDebug() << "built QApplication";
    MainWindow w;
    w.show();

    return app.exec();
}

