#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "message_box.hpp"
#include "parameters/default_parameters_widget.hpp"
#include "parameters/constrained_parameters_widget.hpp"
#include "mcwidget/default_mc_widget.hpp"
#include "mcwidget/constrained_mc_widget.hpp"
#include "utility/logger.hpp"
#include "grid_widget.hpp"
#include "chart_widget.hpp"
#include <QMainWindow>
#include <QtWidgets>
#include <QPushButton>
#include <QToolBar>
#include <QtDebug>
#include <QGroupBox>
#include <QTimer>
#include <QProgressBar>
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
    
public slots:
    void quitAction();

protected:
    QGroupBox* createBottomActionGroup();
    
private:
    GridWidget* gridWidget;
    BaseParametersWidget* prmsWidget;
    BaseMCWidget* MCwidget;
    ChartWidget* hamiltonianChart;
    ChartWidget* averageMagnetisationChart;
    ChartWidget* correlationChart;
    
    // QProgressBar* progressBar;
    QPushButton* quitBtn = new QPushButton("Quit",this);
    
    Ui::MainWindow* ui;
};

