#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "parameters_widget.hpp"
#include "system/montecarlohost.hpp"
#include <QWidget>
#include <QPushButton>
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
    
    void runAction();
    void pauseAction();
    void abortAction();
    void setParameters(ParametersWidget*);
    
public slots:
    void setRunning(bool);
    void makeSystemNew();
    
signals:
    void resetSignal();
    void runningSignal(bool);
    void drawRequest(const MonteCarloHost&, const unsigned long);
    void finishedSteps(const unsigned long);
    
protected:
    
private:
    void server();
    
    ParametersWidget* prmsWidget = Q_NULLPTR;
    QPushButton* runBtn = new QPushButton("Run",this);
    QPushButton* pauseBtn = new QPushButton("Pause",this);
    QPushButton* abortBtn = new QPushButton("Abort",this);
    
    QTimer* drawRequestTimer;
    QTimer* progressTimer;
    
//     QFuture<void>* server_ptr = Q_NULLPTR;
    
    std::atomic<bool> simulation_running {false};
    std::atomic<bool> parameters_linked {false};
    
    MonteCarloHost MC {};
    
    std::atomic<unsigned long> steps_done{0};
};

