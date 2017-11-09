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

    prodBtn->setCheckable(false);
    prodBtn->setChecked(false);
    prodBtn->setMaximumWidth(150);
    
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

    // runBtn->setCheckable(false);
    // runBtn->setChecked(false);
    // runBtn->setMaximumWidth(150);

    // connect(runBtn,       &QPushButton::clicked, this, &MCWidget::runAction);
    connect(equilBtn,     &QPushButton::clicked, this, &MCWidget::equilibrateAction);
    connect(prodBtn,      &QPushButton::clicked, this, &MCWidget::productionAction);
    connect(pauseBtn,     &QPushButton::clicked, this, &MCWidget::pauseAction);
    connect(abortBtn,     &QPushButton::clicked, this, &MCWidget::abortAction);
    connect(saveBtn,      &QPushButton::clicked, this, &MCWidget::saveAction);
    connect(correlateBtn, &QPushButton::clicked, this, &MCWidget::correlateAction);
    connect(drawRequestTimer, &QTimer::timeout, [&]{ emit drawRequest(MC, steps_done.load()); });
    connect(progressTimer,    &QTimer::timeout, [&]{ emit finishedSteps(steps_done.load()); });
    
     // main layout
     QHBoxLayout* mainLayout = new QHBoxLayout;
     Q_CHECK_PTR(mainLayout);
     mainLayout->setAlignment(Qt::AlignHCenter);
 
    mainLayout->addWidget(createStandardMCOptionsBox());
    mainLayout->addWidget(createAdvancedOptionsBox());

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



void MCWidget::makeSystemRandom()
{
    Q_CHECK_PTR(drawRequestTimer);

    steps_done.store(0);
    MC.randomiseSystem();
    MC.clearRecords();
    emit resetChartSignal();
    emit drawRequest(MC, steps_done.load());
}



QGroupBox* MCWidget::createAdvancedOptionsBox()
{
    qDebug() << __PRETTY_FUNCTION__;

    // the group
    QGroupBox* advancedOptionsBox = new QGroupBox("Advanced Simulation Commands");
    Q_CHECK_PTR(advancedOptionsBox);
    
    // // set up the ComboBox:
    // Q_CHECK_PTR(advancedComboBox);
    // advancedComboBox->addItem("B");
    // advancedComboBox->addItem("T");
    // advancedComboBox->addItem("J");

    // // set up the range options:
    // startValueSpinBox->setMinimumWidth(40);
    // startValueSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // stopValueSpinBox->setMinimumWidth(40);
    // stopValueSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // stepValueSpinBox->setMinimumWidth(40);
    // stepValueSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    
    // // the layout 
    // QFormLayout* formLayout = new QFormLayout();
    // Q_CHECK_PTR(formLayout);
    // formLayout->setLabelAlignment(Qt::AlignCenter);
    // formLayout->addRow("produce a range of MC simulations with varying ... ", advancedComboBox);
    
    // QHBoxLayout* rangeOptions = new QHBoxLayout();
    // Q_CHECK_PTR(rangeOptions);
    // rangeOptions->addWidget(startValueSpinBox);
    // rangeOptions->addWidget(stopValueSpinBox);
    // rangeOptions->addWidget(stepValueSpinBox);
    // formLayout->addRow("start : end : step", rangeOptions);
    
    // Q_CHECK_PTR(runBtn);
    // formLayout->addWidget(runBtn);

    // advancedOptionsBox->setLayout(formLayout);
    return advancedOptionsBox;
}




QGroupBox* MCWidget::createStandardMCOptionsBox()
{
    qDebug() << __PRETTY_FUNCTION__;

    // the group
    QGroupBox* standardOptionsBox = new QGroupBox("Standard MC Simulation Commands");
    Q_CHECK_PTR(standardOptionsBox);

    // the layout 
    QHBoxLayout *hbox = new QHBoxLayout;
    Q_CHECK_PTR(hbox);

    hbox->addWidget(equilBtn);
    hbox->addWidget(prodBtn);
    hbox->addWidget(pauseBtn);
    hbox->addWidget(abortBtn);
    hbox->addWidget(saveBtn);
    hbox->addWidget(correlateBtn);

    standardOptionsBox->setLayout(hbox);
    return standardOptionsBox;
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
    // Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    simulation_running.store(true);
    prodBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    // runBtn->setEnabled(false);
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
    
void MCWidget::productionAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    // Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    simulation_running.store(true);
    equilBtn->setEnabled(false);
    prodBtn->setEnabled(false);
    pauseBtn->setEnabled(true);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    // runBtn->setEnabled(false);
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
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    // Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    simulation_running.store(false);
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    // runBtn->setEnabled(false);
    
    emit drawRequest(MC, steps_done.load());
    emit finishedSteps(steps_done.load());
    
    drawRequestTimer->stop();
    progressTimer->stop();
    
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
    // Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    // runBtn->setEnabled(true);
    
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
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    // Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    // runBtn->setEnabled(false);

    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit runningSignal(false);

    MC.print_data();
    MC.print_averages();
}


void MCWidget::correlateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    // Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(true);
    // runBtn->setEnabled(false);

    drawRequestTimer->stop();
    progressTimer->stop();
    
    emit runningSignal(false);

    MC.print_correlation();
}



void MCWidget::runAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(prodBtn);
    Q_CHECK_PTR(pauseBtn);
    Q_CHECK_PTR(saveBtn);
    Q_CHECK_PTR(correlateBtn);
    Q_CHECK_PTR(abortBtn);
    // Q_CHECK_PTR(runBtn);
    Q_CHECK_PTR(drawRequestTimer);
    Q_CHECK_PTR(progressTimer);

    // add code here
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


