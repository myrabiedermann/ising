#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

#include "parameters_widget.hpp"
#include "grid_widget.hpp"
#include "mc_widget.hpp"
#include "chart_widget.hpp"
#include <QtWidgets>
#include <QPushButton>
#include <QToolBar>
#include <QtDebug>
#include <QGroupBox>
#include <QTimer>
#include <QProgressBar>
#include <iostream>
#include <cstdint>


class ConstrainedLayoutWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ConstrainedLayoutWidget(QWidget* parent = Q_NULLPTR);
    ConstrainedLayoutWidget(const ConstrainedLayoutWidget&) = delete;
    void operator=(const ConstrainedLayoutWidget&) = delete;
    
    ~ConstrainedLayoutWidget();

protected:
    QGroupBox* createBottomActionGroup();

private:
    GridWidget* gridWidget;
    ParametersWidget* prmsWidget;
    MCWidget* MCwidget;
    ChartWidget* hamiltonianChart;
    ChartWidget* averageMagnetisationChart;

    // QProgressBar* progressBar;
    QPushButton* quitBtn = new QPushButton("Quit",this);

};