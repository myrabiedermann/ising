#include "mc_widget.hpp"



MCWidget::MCWidget(QWidget *parent) 
  : QWidget(parent)
  , drawRequestTimer(new QTimer(this))
  , progressTimer(new QTimer(this))
{
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
    
    connect(progressTimer, &QTimer::timeout, [&]{ emit finishedSteps(steps_done.load());std::cout << "progress" << std::endl; });
    connect(drawRequestTimer, &QTimer::timeout, [&]{ emit drawRequest(MC); /*std::cout << MC.getSpinsystem() << std::endl;*/ });
}



MCWidget::~MCWidget()
{
}



bool MCWidget::getRunning()
{
    return simulation_running.load();
}



void MCWidget::setRunning(bool b)
{
    simulation_running.store(b);
}



void MCWidget::setParameters(ParametersWidget* widget_ptr)
{
    prmsWidget = widget_ptr;
    assert(prmsWidget);
    
    if(!parameters_linked.load())
    {
        MC.setParameters(prmsWidget);
        MC.setup();
    }
}



void MCWidget::makeSystemNew()
{
    MC.setup();
}





void MCWidget::runAction()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    assert(runBtn);
    assert(pauseBtn);
    assert(abortBtn);
    assert(drawRequestTimer);
    assert(progressTimer);
    
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
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    assert(runBtn);
    assert(pauseBtn);
    assert(abortBtn);
    assert(drawRequestTimer);
    assert(progressTimer);
    
    simulation_running.store(false);
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    
    emit drawRequest(MC);
    emit finishedSteps(steps_done.load());
    
    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit runningSignal(false);
}


void MCWidget::abortAction()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    assert(runBtn);
    assert(pauseBtn);
    assert(abortBtn);
    assert(drawRequestTimer);
    assert(progressTimer);
    
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
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    assert(prmsWidget);
    
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


