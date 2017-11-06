#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "default_layout_widget.hpp"
#include "parameters_widget.hpp"
#include "grid_widget.hpp"
#include "mc_widget.hpp"
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
    

protected:
    
private:
    DefaultLayoutWidget* defaultLayoutWidget;
    
    Ui::MainWindow* ui;
};

