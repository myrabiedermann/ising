#include "base_mc_widget.hpp"


#define BASE_MC_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(equilBtn);  \
    Q_CHECK_PTR(prodBtn);   \
    Q_CHECK_PTR(pauseBtn);  \
    Q_CHECK_PTR(abortBtn);  \
    Q_CHECK_PTR(saveBtn);   \




BaseMCWidget::BaseMCWidget(QWidget *parent) 
  : QWidget(parent)
  , drawRequestTimer(new QTimer(this))
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
    
    if( ! parameters_linked.load() )
    {
        MC.setParameters(prmsWidget);
        MC.setup();
        emit drawRequest(MC, steps_done.load());
    }
}



void BaseMCWidget::makeSystemNew()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(prmsWidget);

    MC.setup();
    steps_done.store(0);
    emit resetChartSignal();
    emit drawRequest(MC, steps_done.load());
}



void BaseMCWidget::makeRecordsNew()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(prmsWidget);

    MC.clearRecords();
    steps_done.store(0);
    emit resetChartSignal();
}



void BaseMCWidget::makeSystemRandom()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(prmsWidget);

    MC.clearRecords();
    MC.resetSpins();
    steps_done.store(0);
    emit resetChartSignal();
    emit drawRequest(MC, steps_done.load());
}



void BaseMCWidget::saveAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    BASE_MC_WIDGET_ASSERT_ALL;

    MC.print_data();
    MC.print_averages();
}



// DO NOT emit from server
void BaseMCWidget::server()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(prmsWidget);
    
    if( equilibration_mode.load() == true )
    {
        while(simulation_running.load() && steps_done.load() < prmsWidget->getStepsEquil())
        {
            MC.run(prmsWidget->getPrintFreq(), true);
            steps_done.store(steps_done.load() + prmsWidget->getPrintFreq());
            
            if( steps_done.load() >= prmsWidget->getStepsEquil() )
            {
                emit pauseBtn->clicked();
            }
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
    
    emit pauseBtn->clicked();
}


