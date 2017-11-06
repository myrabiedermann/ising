#include "mc_widget.hpp"



MCWidget::MCWidget(QWidget *parent) 
  : QWidget(parent)
  , drawRequestTimer(new QTimer(this))
  , progressTimer(new QTimer(this))
{
    MC.setAcceptance(new Metropolis);
    
    qDebug() << __PRETTY_FUNCTION__;
    
    equilBtn->setCheckable(false);
    equilBtn->setChecked(false);
    equilBtn->setMaximumWidth(150);

    runBtn->setCheckable(false);
    runBtn->setChecked(false);
    runBtn->setMaximumWidth(150);
    
    pauseBtn->setCheckable(false);
    pauseBtn->setMaximumWidth(150);
    pauseBtn->setEnabled(false);
    
    abortBtn->setCheckable(false);
    abortBtn->setMaximumWidth(150);
    abortBtn->setEnabled(false);

    saveBtn->setCheckable(false);
    saveBtn->setMaximumWidth(150);
    saveBtn->setEnabled(false);

    correlateBtn->setCheckable(false);
    correlateBtn->setMaximumWidth(150);
    correlateBtn->setEnabled(false);
    

    connect(equilBtn, &QPushButton::clicked, this, &MCWidget::equilibrateAction);
    connect(runBtn,   &QPushButton::clicked, this, &MCWidget::runAction);
    connect(pauseBtn, &QPushButton::clicked, this, &MCWidget::pauseAction);
    connect(abortBtn, &QPushButton::clicked, this, &MCWidget::abortAction);
    connect(saveBtn, &QPushButton::clicked, this, &MCWidget::saveAction);
    connect(correlateBtn, &QPushButton::clicked, this, &MCWidget::correlateAction);
    connect(drawRequestTimer, &QTimer::timeout, [&]{ emit drawRequest(MC, steps_done.load()); });
    connect(progressTimer,    &QTimer::timeout, [&]{ emit finishedSteps(steps_done.load()); });
    

    QHBoxLayout *hbox = new QHBoxLayout;
    Q_CHECK_PTR(hbox);

    hbox->addWidget(equilBtn);
    hbox->addWidget(runBtn);
    hbox->addWidget(pauseBtn);
    hbox->addWidget(abortBtn);
    hbox->addWidget(saveBtn);
    hbox->addWidget(correlateBtn);

    setLayout(hbox);  
}



MCWidget::~MCWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}



bool MCWidget::getRunning()
{
    qDebug() << __PRETTY_FUNCTION__;
    return simulation_running.load();
}



void MCWidget::setRunning(bool b)
{
    qDebug() << __PRETTY_FUNCTION__;
    simulation_running.store(b);
}



void MCWidget::setParameters(ParametersWidget* widget_ptr)
{
    qDebug() << __PRETTY_FUNCTION__;
    prmsWidget = widget_ptr;
    Q_CHECK_PTR(prmsWidget);
    
    if(!parameters_linked.load())
    {
        MC.setParameters(prmsWidget);
        MC.setup();
        emit drawRequest(MC, steps_done.load());
    }
}



void MCWidget::makeSystemNew()
{
    MC.setup();
    steps_done.store(0);
    emit drawRequest(MC, steps_done.load());
}



void MCWidget::makeRecordsNew()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(prmsWidget);

    steps_done.store(0);
    MC.clearRecords();
    emit resetChartSignal();
}



void MCWidget::randomiseSystem()
{
    Q_CHECK_PTR(drawRequestTimer);

    steps_done.store(0);
    MC.randomiseSystem();
    emit resetChartSignal();
    emit drawRequest(MC, steps_done.load());
}



void MCWidget::equilibrateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(equilBtn);
    Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    simulation_running.store(true);
    runBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    drawRequestTimer->start(34);
    progressTimer->start(100);

    if( equilibration_mode.load() == false )
    {
        equilibration_mode.store(true);
        steps_done.store(0);
        emit resetChartSignal();
    }
    emit drawRequest(MC, steps_done.load());

    emit runningSignal(true);
    
    QFuture<void> future = QtConcurrent::run([&]
        {
            server();
        });
    }
    
    void MCWidget::runAction()
    {
        qDebug() << __PRETTY_FUNCTION__;
        
        Q_CHECK_PTR(runBtn);
        Q_CHECK_PTR(pauseBtn);
        Q_CHECK_PTR(saveBtn);
        Q_CHECK_PTR(correlateBtn);
        Q_CHECK_PTR(abortBtn);
        Q_CHECK_PTR(drawRequestTimer);
        Q_CHECK_PTR(progressTimer);
        
        simulation_running.store(true);
        equilBtn->setEnabled(false);
        runBtn->setEnabled(false);
        pauseBtn->setEnabled(true);
        saveBtn->setEnabled(false);
        correlateBtn->setEnabled(false);
        abortBtn->setEnabled(false);
        drawRequestTimer->start(34);
        progressTimer->start(100);
        
        if( equilibration_mode.load() == true )
        {
            equilibration_mode.store(false);
            steps_done.store(0);
            emit resetChartSignal();
        }
        emit drawRequest(MC, steps_done.load());

        emit runningSignal(true);
    
    QFuture<void> future = QtConcurrent::run([&]
    {
        server();
    });
}

void MCWidget::pauseAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    simulation_running.store(false);
    equilBtn->setEnabled(true);
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    
    emit drawRequest(MC, steps_done.load());
    emit finishedSteps(steps_done.load());
    
    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit runningSignal(false);
}


void MCWidget::abortAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    
    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit resetSignal();
    makeSystemNew();
    emit runningSignal(false);
    emit drawRequest(MC, steps_done.load());
}


void MCWidget::saveAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);

    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit runningSignal(false);

    // if( prmsWidget->getPrintData() )
    MC.print_data();
    // if( prmsWidget->getPrintAver() )
    MC.print_averages();
}


void MCWidget::correlateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(true);

    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit runningSignal(false);

    MC.print_correlation();
}




// DO NOT emit from server
void MCWidget::server()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prmsWidget);
    
    if (steps_done.load() >= prmsWidget->getStepsProd() or steps_done.load() >= prmsWidget->getStepsEquil())
        emit pauseBtn->clicked();
    
    if( equilibration_mode.load() == true )
    {
        while(simulation_running.load() && steps_done.load() < prmsWidget->getStepsEquil())
        {
            MC.run(5, true);
            steps_done.store(steps_done.load() + 5);
            
            if( steps_done.load() >= prmsWidget->getStepsEquil() )
                emit pauseBtn->clicked();
        }
    }
    else{
        while(simulation_running.load() && steps_done.load() < prmsWidget->getStepsProd())
        {
            MC.run(prmsWidget->getPrintFreq());
            steps_done.store(steps_done.load() + prmsWidget->getPrintFreq());
            
            if (steps_done.load() >= prmsWidget->getStepsProd())
                emit pauseBtn->clicked();
        }
    }
}


