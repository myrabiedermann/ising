#include "mc_widget.hpp"



MCWidget::MCWidget(QWidget *parent) 
  : QWidget(parent)
  , drawRequestTimer(new QTimer(this))
  , drawCorrelationRequestTimer(new QTimer(this))
//   , progressTimer(new QTimer(this))
{
    MC.setAcceptance(new Metropolis);
    
    qDebug() << __PRETTY_FUNCTION__;
    
    equilBtn->setCheckable(false);
    equilBtn->setChecked(false);
    equilBtn->setMaximumWidth(350);
    equilBtn->setMinimumWidth(150);

    prodBtn->setCheckable(false);
    prodBtn->setChecked(false);
    prodBtn->setMaximumWidth(350);
    prodBtn->setMinimumWidth(150);
    
    pauseBtn->setCheckable(false);
    pauseBtn->setEnabled(false);
    pauseBtn->setMaximumWidth(350);
    pauseBtn->setMinimumWidth(150);
    
    abortBtn->setCheckable(false);
    abortBtn->setEnabled(false);
    abortBtn->setMaximumWidth(350);
    abortBtn->setMinimumWidth(150);

    saveBtn->setCheckable(false);
    saveBtn->setEnabled(false);
    saveBtn->setMaximumWidth(350);
    saveBtn->setMinimumWidth(150);

    correlateBtn->setCheckable(false);
    correlateBtn->setEnabled(false);
    correlateBtn->setMaximumWidth(350);
    correlateBtn->setMinimumWidth(150);

    advancedRunBtn->setCheckable(false);
    advancedRunBtn->setChecked(false);
    advancedRunBtn->setMaximumWidth(350);
    advancedRunBtn->setMinimumWidth(150);

    connect(advancedRunBtn, &QPushButton::clicked, this, &MCWidget::advancedRunAction);
    connect(equilBtn,     &QPushButton::clicked, this, &MCWidget::equilibrateAction);
    connect(prodBtn,      &QPushButton::clicked, this, &MCWidget::productionAction);
    connect(pauseBtn,     &QPushButton::clicked, this, &MCWidget::pauseAction);
    connect(abortBtn,     &QPushButton::clicked, this, &MCWidget::abortAction);
    connect(saveBtn,      &QPushButton::clicked, this, &MCWidget::saveAction);
    connect(correlateBtn, &QPushButton::clicked, this, &MCWidget::correlateAction);
    connect(drawRequestTimer, &QTimer::timeout, [&]{ emit drawRequest(MC, steps_done.load()); });
    connect(drawCorrelationRequestTimer, &QTimer::timeout, [&]{ emit drawCorrelationRequest(MC); });
    
    // main layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    Q_CHECK_PTR(mainLayout);
    mainLayout->setAlignment(Qt::AlignHCenter);
 
    Q_CHECK_PTR(equilBtn);
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(advancedRunBtn);
    
    mainLayout->addWidget(equilBtn);
    mainLayout->addWidget(prodBtn);
    mainLayout->addWidget(advancedRunBtn);
    mainLayout->addWidget(pauseBtn);
    mainLayout->addWidget(abortBtn);
    mainLayout->addWidget(saveBtn);
    mainLayout->addWidget(correlateBtn);

    setLayout(mainLayout);  

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



void MCWidget::setParameters(BaseParametersWidget* widget_ptr)
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
    steps_done.store(0);
    MC.setup();
    emit resetChartSignal();
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



void MCWidget::makeSystemRandom()
{
    Q_CHECK_PTR(drawRequestTimer);

    steps_done.store(0);
    MC.resetSpins();
    MC.clearRecords();
    emit resetChartSignal();
    emit drawRequest(MC, steps_done.load());
}



void MCWidget::equilibrateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(equilBtn);
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(advancedRunBtn);
    Q_CHECK_PTR(drawRequestTimer);
    
    setRunning(true);
    
    if( ! advanced_mode )
    { 
        pauseBtn->setEnabled(true);
    }
    else
    {
        pauseBtn->setEnabled(false);
    }
    
    prodBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);
    drawRequestTimer->start(100);
    // progressTimer->start(100);

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
    

void MCWidget::productionAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(advancedRunBtn);
    Q_CHECK_PTR(drawRequestTimer);
    
    setRunning(true);
    
    if( ! advanced_mode )
    { 
        pauseBtn->setEnabled(true);
    }
    else
    {
        pauseBtn->setEnabled(false);
    }
    
    equilBtn->setEnabled(false);
    prodBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);
    
    drawRequestTimer->start(100);
    
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
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(advancedRunBtn);
    Q_CHECK_PTR(drawRequestTimer);
    
    setRunning(false);
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    advancedRunBtn->setEnabled(true);
    
    emit drawRequest(MC, steps_done.load());
    emit finishedSteps(steps_done.load());
    
    drawRequestTimer->stop();
    
    emit runningSignal(false);
}


void MCWidget::abortAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(advancedRunBtn);
    Q_CHECK_PTR(drawRequestTimer);
    // Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    advancedRunBtn->setEnabled(true);
    
    drawRequestTimer->stop();
    
    emit resetSignal();
    makeSystemNew();
    emit runningSignal(false);
    emit drawRequest(MC, steps_done.load());
}


void MCWidget::saveAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(advancedRunBtn);
    Q_CHECK_PTR(drawRequestTimer);
    // Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);

    drawRequestTimer->stop();
    
    emit runningSignal(false);

    MC.print_data();
    MC.print_averages();
    if( prmsWidget->getConstrained() ) MC.print_amplitudes();
}


void MCWidget::correlateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(advancedRunBtn);
    Q_CHECK_PTR(drawRequestTimer);
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);
    
    MC.print_correlation(); 
    emit drawCorrelationRequest(MC);
}



void MCWidget::advancedRunAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    Q_CHECK_PTR(advancedRunBtn);
    Q_CHECK_PTR(drawRequestTimer);

    advanced_mode = true;

    if( prmsWidget->getStopValue() != prmsWidget->getStartValue() )
    {
        unsigned int nrsteps = static_cast<unsigned int>( ( prmsWidget->getStopValue() - prmsWidget->getStartValue() ) / prmsWidget->getStepValue() );   
    
        for(unsigned int stepper = 0; stepper <= nrsteps; ++stepper)
        {
            prmsWidget->setAdvancedValue( prmsWidget->getStartValue() + static_cast<double>(prmsWidget->getStepValue() * stepper) );
            {
                QEventLoop pause;
                connect(this, &MCWidget::serverReturn, &pause, &QEventLoop::quit);
                equilibrateAction();
                pause.exec();
            }
            {
                QEventLoop pause;
                connect(this, &MCWidget::serverReturn, &pause, &QEventLoop::quit);
                productionAction();
                pause.exec();
            }
            saveAction();
            makeRecordsNew();
        }
    }
    
    advanced_mode = false;
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);

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
            MC.run(prmsWidget->getPrintFreq(), true);
            steps_done.store(steps_done.load() + prmsWidget->getPrintFreq());
            
            if( steps_done.load() >= prmsWidget->getStepsEquil() )
                emit pauseBtn->clicked();
        }
    }
    else
    {
        while(simulation_running.load() && steps_done.load() < prmsWidget->getStepsProd())
        {
            MC.run(prmsWidget->getPrintFreq(), false);
            steps_done.store(steps_done.load() + prmsWidget->getPrintFreq());
            
            if (steps_done.load() >= prmsWidget->getStepsProd())
            {
                emit pauseBtn->clicked();
            }
        }
    }
    emit serverReturn();
}


