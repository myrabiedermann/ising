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
    equilBtn->setMinimumHeight(10);
    equilBtn->setMaximumHeight(30);
    equilBtn->setFocusPolicy(Qt::NoFocus);

    prodBtn->setCheckable(false);
    prodBtn->setEnabled(true);
    prodBtn->setMaximumWidth(350);
    prodBtn->setMinimumWidth(150);
    prodBtn->setMinimumHeight(10);
    prodBtn->setMaximumHeight(30);
    prodBtn->setFocusPolicy(Qt::NoFocus);
    
    pauseBtn->setCheckable(false);
    pauseBtn->setEnabled(false);
    pauseBtn->setMaximumWidth(350);
    pauseBtn->setMinimumWidth(150);
    pauseBtn->setMinimumHeight(10);
    pauseBtn->setMaximumHeight(30);
    pauseBtn->setFocusPolicy(Qt::NoFocus);
    
    abortBtn->setCheckable(false);
    abortBtn->setEnabled(false);
    abortBtn->setMaximumWidth(350);
    abortBtn->setMinimumWidth(150);
    abortBtn->setMinimumHeight(10);
    abortBtn->setMaximumHeight(30);
    abortBtn->setFocusPolicy(Qt::NoFocus);

    saveBtn->setCheckable(false);
    saveBtn->setEnabled(false);
    saveBtn->setMaximumWidth(350);
    saveBtn->setMinimumWidth(150);
    saveBtn->setMinimumHeight(10);
    saveBtn->setMaximumHeight(30);
    saveBtn->setFocusPolicy(Qt::NoFocus);

    advancedRunBtn->setCheckable(false);
    advancedRunBtn->setEnabled(true);
    advancedRunBtn->setMaximumWidth(350);
    advancedRunBtn->setMinimumWidth(150);
    advancedRunBtn->setMinimumHeight(10);
    advancedRunBtn->setMaximumHeight(30);
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

    isingLOG("gui: " << "start equilibration with " << prmsWidget->getStepsEquil() - steps_done.load() << " steps")
    
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
    
    isingLOG("gui: " << "start production with " << prmsWidget->getStepsProd() - steps_done.load() << " steps")

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

    isingLOG("gui: " << "pausing @ step " << steps_done.load())
}



void DefaultMCWidget::abortAction() 
{
    qDebug() << __PRETTY_FUNCTION__;
    
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    
    setRunning(false);
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    advancedRunBtn->setEnabled(true);

    drawRequestTimer->stop();
    
    emit runningSignal(false);
    emit resetSignal();
    makeSystemNew();

    isingLOG("gui: " << "abort & reset to default parameters")
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

    setRunning(true);
    emit runningSignal(true);

    emit drawRequest(MC, steps_done.load());
    drawRequestTimer->start(drawRequestTime.load());

    isingLOG("gui: " << "start running advanced scheme")

    MC.clearRecords();

    double value = prmsWidget->getStartValue();
    int factor = (prmsWidget->getStepValue() < 0 ? -1 : 1);
    double finalValue = prmsWidget->getStopValue() + 0.5*factor*prmsWidget->getStepValue();
    while( factor*(value - finalValue) <= 0)
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

    setRunning(false);
    emit runningSignal(false);
    drawRequestTimer->stop();

    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    advancedRunBtn->setEnabled(true);

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
}



DefaultMCWidget::~DefaultMCWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}


    
