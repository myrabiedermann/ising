#pragma once


#include "parameters_widget.hpp"
#include "grid_widget.hpp"
#include "mc_widget.hpp"
#include <QMainWindow>
#include <QtWidgets>
#include <QPushButton>
#include <QGroupBox>
#include <QTimer>
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
    
//     void runAction();

protected:
    QGroupBox* createBottomActionGroup();
    
private:
    
    GridWidget* gridWidget;
    ParametersWidget* prmsWidget;
    
    MCWidget* MCwidget = new MCWidget(this);
    QPushButton* quitBtn = new QPushButton("Quit",this);
    
    Ui::MainWindow* ui;
};

