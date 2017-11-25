#include "base_mc_widget.hpp"


#define BASE_MC_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(equilBtn);  \
    Q_CHECK_PTR(prodBtn);   \
    Q_CHECK_PTR(pauseBtn);  \
    Q_CHECK_PTR(abortBtn);  \
    Q_CHECK_PTR(saveBtn);   \
    Q_CHECK_PTR(correlateBtn);  \
    Q_CHECK_PTR(drawRequestTimer);\
    Q_CHECK_PTR(drawCorrelationRequestTimer);




BaseMCWidget::BaseMCWidget(QWidget *parent) 
  : QWidget(parent)
  , drawRequestTimer(new QTimer(this))
  , drawCorrelationRequestTimer(new QTimer(this))
{
    qDebug() << __PRETTY_FUNCTION__;
}



BaseMCWidget::~BaseMCWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}



bool BaseMCWidget::getRunning()
{
    qDebug() << __PRETTY_FUNCTION__;
    return simulation_running.load();
}



void BaseMCWidget::setRunning(bool b)
{
    qDebug() << __PRETTY_FUNCTION__;
    simulation_running.store(b);
}



void BaseMCWidget::setParameters(BaseParametersWidget* widget_ptr)
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



void BaseMCWidget::makeSystemNew()
{
    steps_done.store(0);
    MC.setup();
    emit resetChartSignal();
    emit drawRequest(MC, steps_done.load());
}



void BaseMCWidget::makeRecordsNew()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(prmsWidget);

    steps_done.store(0);
    MC.clearRecords();
    emit resetChartSignal();
}



void BaseMCWidget::makeSystemRandom()
{
    Q_CHECK_PTR(drawRequestTimer);

    steps_done.store(0);
    MC.resetSpins();
    MC.clearRecords();
    emit resetChartSignal();
    emit drawRequest(MC, steps_done.load());
}



void BaseMCWidget::equilibrateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    BASE_MC_WIDGET_ASSERT_ALL;
    
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
    // advancedRunBtn->setEnabled(false);
    drawRequestTimer->start(100);

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
    

void BaseMCWidget::productionAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    BASE_MC_WIDGET_ASSERT_ALL;
    
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
    // advancedRunBtn->setEnabled(false);
    
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


void BaseMCWidget::pauseAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    BASE_MC_WIDGET_ASSERT_ALL;
    
    setRunning(false);
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    // advancedRunBtn->setEnabled(true);
    
    emit drawRequest(MC, steps_done.load());
    emit finishedSteps(steps_done.load());
    
    drawRequestTimer->stop();
    
    emit runningSignal(false);
}


void BaseMCWidget::abortAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    BASE_MC_WIDGET_ASSERT_ALL;
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    // advancedRunBtn->setEnabled(true);
    
    drawRequestTimer->stop();
    
    emit resetSignal();
    makeSystemNew();
    emit runningSignal(false);
    emit drawRequest(MC, steps_done.load());
}


void BaseMCWidget::saveAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    BASE_MC_WIDGET_ASSERT_ALL;
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    // advancedRunBtn->setEnabled(false);

    drawRequestTimer->stop();
    
    emit runningSignal(false);

    MC.print_data();
    MC.print_averages();
    // if( prmsWidget->getConstrained() ) MC.print_amplitudes();
}


void BaseMCWidget::correlateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    BASE_MC_WIDGET_ASSERT_ALL;
    
    // MC.print_correlation(); 
    emit drawCorrelationRequest(MC);
}



// DO NOT emit from server
void BaseMCWidget::server()
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


