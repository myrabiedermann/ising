#include "default_mc_widget.hpp"

#define DEFAULT_MC_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(equilBtn);  \
    Q_CHECK_PTR(prodBtn);   \
    Q_CHECK_PTR(pauseBtn);  \
    Q_CHECK_PTR(abortBtn);  \
    Q_CHECK_PTR(saveBtn);   \
    Q_CHECK_PTR(advancedRunBtn); \
    Q_CHECK_PTR(drawRequestTimer);




DefaultMCWidget::DefaultMCWidget(QWidget *parent) 
  : BaseMCWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    
    equilBtn->setCheckable(false);
    equilBtn->setEnabled(true);
    equilBtn->setMaximumWidth(350);
    equilBtn->setMinimumWidth(150);
    equilBtn->setFocusPolicy(Qt::NoFocus);

    prodBtn->setCheckable(false);
    prodBtn->setEnabled(true);
    prodBtn->setMaximumWidth(350);
    prodBtn->setMinimumWidth(150);
    prodBtn->setFocusPolicy(Qt::NoFocus);
    
    pauseBtn->setCheckable(false);
    pauseBtn->setEnabled(false);
    pauseBtn->setMaximumWidth(350);
    pauseBtn->setMinimumWidth(150);
    pauseBtn->setFocusPolicy(Qt::NoFocus);
    
    abortBtn->setCheckable(false);
    abortBtn->setEnabled(false);
    abortBtn->setMaximumWidth(350);
    abortBtn->setMinimumWidth(150);
    abortBtn->setFocusPolicy(Qt::NoFocus);

    saveBtn->setCheckable(false);
    saveBtn->setEnabled(false);
    saveBtn->setMaximumWidth(350);
    saveBtn->setMinimumWidth(150);
    saveBtn->setFocusPolicy(Qt::NoFocus);

    advancedRunBtn->setCheckable(false);
    advancedRunBtn->setEnabled(true);
    advancedRunBtn->setMaximumWidth(350);
    advancedRunBtn->setMinimumWidth(150);
    advancedRunBtn->setFocusPolicy(Qt::NoFocus);

    connect(advancedRunBtn, &QPushButton::clicked, this, &DefaultMCWidget::advancedRunAction);
    connect(equilBtn,       &QPushButton::clicked, this, &BaseMCWidget::equilibrateAction);
    connect(prodBtn,        &QPushButton::clicked, this, &BaseMCWidget::productionAction);
    connect(pauseBtn,       &QPushButton::clicked, this, &BaseMCWidget::pauseAction);
    connect(abortBtn,       &QPushButton::clicked, this, &BaseMCWidget::abortAction);
    connect(saveBtn,        &QPushButton::clicked, this, &BaseMCWidget::saveAction);
    connect(drawRequestTimer, &QTimer::timeout, [&]{ emit drawRequest(MC, steps_done.load()); });
    
    // main layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    Q_CHECK_PTR(mainLayout);
    mainLayout->setAlignment(Qt::AlignHCenter);
 
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    
    mainLayout->addWidget(equilBtn);
    mainLayout->addWidget(prodBtn);
    mainLayout->addWidget(advancedRunBtn);
    mainLayout->addWidget(pauseBtn);
    mainLayout->addWidget(abortBtn);
    mainLayout->addWidget(saveBtn);

    setLayout(mainLayout);
}



void DefaultMCWidget::equilibrateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    
    setRunning(true);
    
    pauseBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);
    equilBtn->setEnabled(false);
    prodBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    abortBtn->setEnabled(false);    

    if( equilibration_mode.load() == false )
    {
        equilibration_mode.store(true);
        steps_done.store(0);
        emit resetChartSignal();
    }

    emit drawRequest(MC, steps_done.load());
    drawRequestTimer->start(drawRequestTime.load());
    emit runningSignal(true);
    
    QFuture<void> future = QtConcurrent::run([&]
    {
        server();
    });
}



void DefaultMCWidget::productionAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    
    setRunning(true);
    
    pauseBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);
    equilBtn->setEnabled(false);
    prodBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    
    if( equilibration_mode.load() == true )
    {
        equilibration_mode.store(false);
        steps_done.store(0);
        emit resetChartSignal();
    }

    emit drawRequest(MC, steps_done.load());
    drawRequestTimer->start(drawRequestTime.load());
    emit runningSignal(true);
    
    QFuture<void> future = QtConcurrent::run([&]
    {
        server();
    });
}



void DefaultMCWidget::pauseAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    
    setRunning(false);
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    advancedRunBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    
    emit runningSignal(false);
    drawRequestTimer->stop();
    emit drawRequest(MC, steps_done.load());
}



void DefaultMCWidget::abortAction() 
{
    qDebug() << __PRETTY_FUNCTION__;
    
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    
    setRunning(false);
    // emit abortSignal();
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    advancedRunBtn->setEnabled(true);

    // advancedCycleDone.store(true);
    // advancedEquilMode.store(true);
    // advancedValues.clear();
    
    drawRequestTimer->stop();
    
    emit runningSignal(false);
    emit resetSignal();
    makeSystemNew();
}



void DefaultMCWidget::advancedRunAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    DEFAULT_MC_WIDGET_ASSERT_ALL;

    equilBtn->setEnabled(false);
    prodBtn->setEnabled(false);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    advancedRunBtn->setEnabled(false);

    // advancedValues.clear();
    // double value = prmsWidget->getStopValue();
    // advancedValues.push_back(value);
    // while( value > prmsWidget->getStartValue() + prmsWidget->getStepValue() )
    // {
    //     value -= prmsWidget->getStepValue();
    //     advancedValues.push_back(value);
    // }
    // advancedValues.push_back( prmsWidget->getStartValue() );

    // setRunning(true);
    // emit runningSignal(true);

    // steps_done.store(0);
    // emit resetChartSignal();

    // emit drawRequest(MC, steps_done.load());
    // drawRequestTimer->start(drawRequestTime.load());
    
    // QFuture<void> future = QtConcurrent::run([&]
    // {
    //     serverAdvanced();
    // });

    setRunning(true);
    emit runningSignal(true);

    emit drawRequest(MC, steps_done.load());
    drawRequestTimer->start(drawRequestTime.load());

    MC.clearRecords();

    double value = prmsWidget->getStartValue();
    while( value <= prmsWidget->getStopValue() )
    {
        prmsWidget->setAdvancedValue(value);
        if( prmsWidget->getAdvancedRandomise() )
        {
            MC.resetSpins();
        }
        steps_done.store(0);
        emit resetChartSignal();
        equilibration_mode.store(true);
        {
            QEventLoop pause;
            connect(this, &DefaultMCWidget::serverReturn, &pause, &QEventLoop::quit);
            QFuture<void> future = QtConcurrent::run([&]
            {
                serverAdvanced();
            });
            pause.exec();
        }

        steps_done.store(0);
        equilibration_mode.store(false);
        emit resetChartSignal();
        {
            QEventLoop pause;
            connect(this, &DefaultMCWidget::serverReturn, &pause, &QEventLoop::quit);
            QFuture<void> future = QtConcurrent::run([&]
            {
                serverAdvanced();
            });
            pause.exec();
        }
        MC.print_averages();
        MC.clearRecords();

        value += prmsWidget->getStepValue();
    }

    emit abortBtn->clicked();
}



void DefaultMCWidget::serverAdvanced()
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
                emit serverReturn();
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
                emit serverReturn();
            }
        }
    }
    emit serverReturn();
    // qDebug() << __PRETTY_FUNCTION__;
    // Q_CHECK_PTR(prmsWidget);
    
    // while( advancedValues.size() > 0 && simulation_running.load() )
    // {

    //     if( advancedCycleDone.load() == true )
    //     {
    //         prmsWidget->setAdvancedValue( advancedValues.back() );
    //         advancedValues.pop_back();
    //         advancedCycleDone.store(false);
    //         // makeSystemRandom();
    //     }

    //     while( simulation_running.load() && advancedEquilMode.load() == true && steps_done.load() < prmsWidget->getStepsEquil() )
    //     {
    //         MC.run(prmsWidget->getPrintFreq(), true);
    //         steps_done.store(steps_done.load() + prmsWidget->getPrintFreq());
    //         if( steps_done.load() >= prmsWidget->getStepsEquil() )
    //         {
    //             advancedEquilMode.store(false);
    //             steps_done.store(0);
    //             emit resetChartSignal();
    //         }
    //     }

    //     while( simulation_running.load() && advancedEquilMode.load() == false && steps_done.load() < prmsWidget->getStepsProd() )
    //     {
    //         MC.run(prmsWidget->getPrintFreq(), false);
    //         steps_done.store(steps_done.load() + prmsWidget->getPrintFreq());
    //         if( steps_done.load() >= prmsWidget->getStepsProd() )
    //         {
    //             advancedEquilMode.store(true);
    //             advancedCycleDone.store(true);
    //             steps_done.store(0);
    //             MC.print_averages();
    //             MC.clearRecords();
    //             MC.resetSpins();
    //             emit resetChartSignal();
    //         }
    //     }
    // }
    // emit pauseBtn->clicked();
}



DefaultMCWidget::~DefaultMCWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}


    
