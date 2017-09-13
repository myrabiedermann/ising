#include "mc_widget.hpp"



MCWidget::MCWidget(QWidget *parent) 
  : QWidget(parent)
  , drawRequestTimer(new QTimer(this))
  , progressTimer(new QTimer(this))
{
    qDebug() << __PRETTY_FUNCTION__;
    QHBoxLayout *hbox = new QHBoxLayout;
    
    runBtn->setCheckable(false);
    runBtn->setChecked(false);
    runBtn->setMaximumWidth(100);
    
    pauseBtn->setCheckable(false);
    pauseBtn->setMaximumWidth(100);
    pauseBtn->setEnabled(false);
    
    abortBtn->setCheckable(false);
    abortBtn->setMaximumWidth(100);
    abortBtn->setEnabled(false);
    
    connect(runBtn,   &QPushButton::clicked, this, &MCWidget::runAction);
    connect(pauseBtn, &QPushButton::clicked, this, &MCWidget::pauseAction);
    connect(abortBtn, &QPushButton::clicked, this, &MCWidget::abortAction);
    
    hbox->addWidget(runBtn);
    hbox->addWidget(pauseBtn);
    hbox->addWidget(abortBtn);
    
    setLayout(hbox);  
    
    connect(drawRequestTimer, &QTimer::timeout, [&]{ emit drawRequest(MC, steps_done.load()); });
    connect(progressTimer,    &QTimer::timeout, [&]{ emit finishedSteps(steps_done.load()); });
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
    }
}



void MCWidget::makeSystemNew()
{
    MC.setup();
    steps_done.store(0);
    emit drawRequest(MC, steps_done.load());
}



void MCWidget::runAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    simulation_running.store(true);
    runBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    abortBtn->setEnabled(false);
    drawRequestTimer->start(34);
    progressTimer->start(100);
    
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
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
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
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    
    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit runningSignal(false);
}



// DO NOT emit from server
void MCWidget::server()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prmsWidget);
    
    if (steps_done.load() >= prmsWidget->getSteps())
        emit pauseBtn->clicked();
    
    while(simulation_running.load() && steps_done.load() < prmsWidget->getSteps())
    {
        MC.do_metropolis(prmsWidget->getPrintFreq());
        steps_done.store(steps_done.load() + prmsWidget->getPrintFreq());
        
        if (steps_done.load() >= prmsWidget->getSteps())
            emit pauseBtn->clicked();
    }
}


