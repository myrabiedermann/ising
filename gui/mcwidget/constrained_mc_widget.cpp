#include "constrained_mc_widget.hpp"

#define CONSTRAINED_MC_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(equilBtn);  \
    Q_CHECK_PTR(prodBtn);   \
    Q_CHECK_PTR(pauseBtn);  \
    Q_CHECK_PTR(abortBtn);  \
    Q_CHECK_PTR(saveBtn);   \
    Q_CHECK_PTR(correlateBtn);  \
    Q_CHECK_PTR(drawRequestTimer);




ConstrainedMCWidget::ConstrainedMCWidget(QWidget *parent) 
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

    correlateBtn->setCheckable(false);
    correlateBtn->setEnabled(true);
    correlateBtn->setMaximumWidth(350);
    correlateBtn->setMinimumWidth(150);
    correlateBtn->setMinimumHeight(10);
    correlateBtn->setMaximumHeight(30);
    correlateBtn->setFocusPolicy(Qt::NoFocus);

    connect(equilBtn,     &QPushButton::clicked, this, &BaseMCWidget::equilibrateAction);
    connect(prodBtn,      &QPushButton::clicked, this, &BaseMCWidget::productionAction);
    connect(pauseBtn,     &QPushButton::clicked, this, &BaseMCWidget::pauseAction);
    connect(abortBtn,     &QPushButton::clicked, this, &BaseMCWidget::abortAction);
    connect(saveBtn,      &QPushButton::clicked, this, &BaseMCWidget::saveAction);
    connect(correlateBtn, &QPushButton::clicked, this, &ConstrainedMCWidget::correlateAction);
    connect(drawRequestTimer, &QTimer::timeout, [&]{ emit drawRequest(MC, steps_done.load()); });
    
    // main layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    Q_CHECK_PTR(mainLayout);
    mainLayout->setAlignment(Qt::AlignHCenter);
 
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;
    
    mainLayout->addWidget(equilBtn);
    mainLayout->addWidget(prodBtn);
    mainLayout->addWidget(pauseBtn);
    mainLayout->addWidget(abortBtn);
    mainLayout->addWidget(saveBtn);
    mainLayout->addWidget(correlateBtn);

    setLayout(mainLayout); 
}



void ConstrainedMCWidget::equilibrateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;
    
    setRunning(true);
    
    pauseBtn->setEnabled(true);
    prodBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);

    drawRequestTimer->start(drawRequestTime.load());

    if( equilibration_mode.load() == false )
    {
        equilibration_mode.store(true);
        steps_done.store(0);
        emit resetChartSignal();
    }
    emit drawRequest(MC, steps_done.load());
    emit runningSignal(true);

    isingLOG("gui: " << "start equilibration with " << prmsWidget->getStepsEquil() - steps_done.load() << " steps")
    
    QFuture<void> future = QtConcurrent::run([&]
    {
        server();
    });
}



void ConstrainedMCWidget::productionAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;
    
    setRunning(true);
    
    pauseBtn->setEnabled(true);
    equilBtn->setEnabled(false);
    prodBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(false);
    
    drawRequestTimer->start(drawRequestTime.load());
    
    if( equilibration_mode.load() == true )
    {
        equilibration_mode.store(false);
        steps_done.store(0);
        emit resetChartSignal();
    }
    emit drawRequest(MC, steps_done.load());
    emit runningSignal(true);
    
    isingLOG("gui: " << "start production with " << prmsWidget->getStepsProd() - steps_done.load() << " steps")

    QFuture<void> future = QtConcurrent::run([&]
    {
        server();
    });
}



void ConstrainedMCWidget::pauseAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;
    
    setRunning(false);
    emit runningSignal(false);

    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);
    
    emit drawRequest(MC, steps_done.load());
    
    drawRequestTimer->stop();
    
    isingLOG("gui: " << "pausing @ step " << steps_done.load())
}



void ConstrainedMCWidget::abortAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(false);
    
    drawRequestTimer->stop();
    
    emit runningSignal(false);
    emit resetSignal();
    makeSystemNew();
    isingLOG("gui: " << "abort & reset to default parameters")
}



void ConstrainedMCWidget::correlateAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;

    setRunning(true);
    emit runningSignal(true);

    equilBtn->setEnabled(false);
    prodBtn->setEnabled(false);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(false);
    abortBtn->setEnabled(true);

    isingLOG("gui: " << "computing correlation...")

    QFuture<void> future = QtConcurrent::run([&]
    {
        serverCorrelation();
    });
 
}


void ConstrainedMCWidget::serverCorrelation()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;
    Q_CHECK_PTR(prmsWidget);
    
    auto correlation = MC.getSpinsystem().computeCorrelation();
    MC.print_correlation( correlation );
    auto structureFunction = MC.getSpinsystem().computeStructureFunction(correlation);
    MC.print_structureFunction( structureFunction );
    emit drawCorrelationRequest( correlation );

    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(true);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(true);

    emit runningSignal(false);
    isingLOG("gui: " << "...done")
}



ConstrainedMCWidget::~ConstrainedMCWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}
