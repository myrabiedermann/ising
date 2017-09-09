#include "mc_widget.hpp"



MCWidget::MCWidget(QWidget *parent) 
: QWidget(parent)
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
}



MCWidget::~MCWidget()
{
    delete runBtn;
    delete pauseBtn;
    delete abortBtn;
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




void MCWidget::runAction()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    simulation_running.store(true);
    runBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    abortBtn->setEnabled(false);
    emit runningSignal(true);
    
    auto future = QtConcurrent::run([&]
    {
        server();
    });
}

void MCWidget::pauseAction()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    simulation_running.store(false);
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    emit runningSignal(false);
}


void MCWidget::abortAction()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    runBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    emit runningSignal(false);
}



void MCWidget::server()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    
    assert(prmsWidget);
    while(simulation_running.load())
    {
        MC.do_metropolis(1);
        emit drawRequest(MC);
    }
}


