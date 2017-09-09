#pragma once

#include "parameters_widget.hpp"
#include "system/MC.hpp"
#include <QWidget>
#include <QPushButton>
#include <QtConcurrent/QtConcurrent>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QEvent>
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
    
    void runAction();
    void pauseAction();
    void abortAction();
    void setParameters(ParametersWidget*);
    
public slots:
    void setRunning(bool);
    
signals:
    void runningSignal(bool);
    void drawRequest(const mc&);
    void finishedAllSteps();
    
protected:
    
private:
    void server();
    
    ParametersWidget* prmsWidget = Q_NULLPTR;
    QPushButton* runBtn = new QPushButton("Run",this);
    QPushButton* pauseBtn = new QPushButton("Pause",this);
    QPushButton* abortBtn = new QPushButton("Abort",this);
    
    std::atomic<bool> simulation_running {false};
    std::atomic<bool> parameters_linked {false};
    
    mc MC {};
};

