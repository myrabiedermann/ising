#ifndef ISING_H
#define ISING_H


#include "parameters_widget.hpp"
#include "grid_widget.hpp"
#include <QMainWindow>
#include <QtWidgets>
#include <QPushButton>
#include <QGroupBox>
// #include <QThread>
// #include <QCoreApplication>
// #include <QtCore>
// #include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <cstdint>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    MainWindow(const MainWindow&) = delete;
    void operator=(const MainWindow&) = delete;
    ~MainWindow();
    
    void runAction();

protected:
    QGroupBox* createBottomActionGroup();
    
private:
    
    GridWidget* gridWidget = new GridWidget(this);
    ParametersWidget* prmsWidget = new ParametersWidget(this);
    
    QPushButton* runBtn = new QPushButton("Run",this);
    QPushButton* quitBtn = new QPushButton("Quit",this);
    
    Ui::MainWindow* ui;
};

#endif // ISING_H
