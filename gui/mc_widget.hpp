#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "default_parameters_widget.hpp"
#include "system/montecarlohost.hpp"
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QtConcurrent/QtConcurrent>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QtDebug>
#include <QEvent>
#include <QTimer>
#include <iostream>
#include <atomic>


class MCWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MCWidget(QWidget* parent = Q_NULLPTR);
    MCWidget(const MCWidget&) = delete;
    void operator=(const MCWidget&) = delete;
    
    ~MCWidget();
    
    bool getRunning();
    
    void equilibrateAction();
    void productionAction();
    void pauseAction();
    void abortAction();
    void saveAction();
    void correlateAction();
    void advancedRunAction();

    void setParameters(BaseParametersWidget*);
    
public slots:
    void setRunning(bool);
    void makeSystemNew();
    void makeRecordsNew();
    void makeSystemRandom();
    
signals:
    void resetSignal();
    void resetChartSignal();
    void runningSignal(bool);
    void drawRequest(const MonteCarloHost&, const unsigned long);
    void finishedSteps(const unsigned long);
    void serverReturn();
    
protected:
    
private:
    void server();
    
    BaseParametersWidget* prmsWidget = Q_NULLPTR;
    QPushButton* equilBtn = new QPushButton("Equilibration Run",this);
    QPushButton* prodBtn = new QPushButton("Production Run",this);
    QPushButton* pauseBtn = new QPushButton("Pause",this);
    QPushButton* abortBtn = new QPushButton("Abort",this);
    QPushButton* saveBtn = new QPushButton("Save sample data",this);
    QPushButton* correlateBtn = new QPushButton("Save correlation",this);
    QPushButton* advancedRunBtn = new QPushButton("Advanced Simulation Scheme", this);
    
    QTimer* drawRequestTimer;
    QTimer* progressTimer;
    
    std::atomic<bool> equilibration_mode {false};
    std::atomic<bool> advanced_mode {false};
    std::atomic<bool> simulation_running {false};
    std::atomic<bool> parameters_linked {false};
    
    MonteCarloHost MC {};
    
    std::atomic<unsigned long> steps_done {0};
};

