#include "default_mc_widget.hpp"

#define DEFAULT_MC_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(equilBtn);  \
    Q_CHECK_PTR(prodBtn);   \
    Q_CHECK_PTR(pauseBtn);  \
    Q_CHECK_PTR(abortBtn);  \
    Q_CHECK_PTR(saveBtn);   \
    Q_CHECK_PTR(correlateBtn);  \
    Q_CHECK_PTR(advancedRunBtn); \
    Q_CHECK_PTR(drawRequestTimer);




DefaultMCWidget::DefaultMCWidget(QWidget *parent) 
  : BaseMCWidget(parent)
{
      qDebug() << __PRETTY_FUNCTION__;
      setup();
}



void DefaultMCWidget::setup()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    equilBtn->setCheckable(false);
    equilBtn->setEnabled(true);
    equilBtn->setMaximumWidth(350);
    equilBtn->setMinimumWidth(150);

    prodBtn->setCheckable(false);
    prodBtn->setEnabled(true);
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
    correlateBtn->setEnabled(true);
    correlateBtn->setMaximumWidth(350);
    correlateBtn->setMinimumWidth(150);

    advancedRunBtn->setCheckable(false);
    advancedRunBtn->setEnabled(true);
    advancedRunBtn->setMaximumWidth(350);
    advancedRunBtn->setMinimumWidth(150);

    connect(advancedRunBtn, &QPushButton::clicked, this, &BaseMCWidget::advancedRunAction);
    connect(equilBtn,     &QPushButton::clicked, this, &BaseMCWidget::equilibrateAction);
    connect(prodBtn,      &QPushButton::clicked, this, &BaseMCWidget::productionAction);
    connect(pauseBtn,     &QPushButton::clicked, this, &BaseMCWidget::pauseAction);
    connect(abortBtn,     &QPushButton::clicked, this, &BaseMCWidget::abortAction);
    connect(saveBtn,      &QPushButton::clicked, this, &BaseMCWidget::saveAction);
    connect(correlateBtn, &QPushButton::clicked, this, &BaseMCWidget::correlateAction);
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
    mainLayout->addWidget(correlateBtn);

    setLayout(mainLayout);  
}



void DefaultMCWidget::abortAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    
    equilBtn->setEnabled(true);
    prodBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    saveBtn->setEnabled(false);
    correlateBtn->setEnabled(true);
    abortBtn->setEnabled(false);
    advancedRunBtn->setEnabled(true);
    
    drawRequestTimer->stop();
    
    emit runningSignal(false);
    emit resetSignal();
    makeSystemNew();
    emit drawRequest(MC, steps_done.load());
}



void DefaultMCWidget::advancedRunAction()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    DEFAULT_MC_WIDGET_ASSERT_ALL;
    advanced_mode = true;

    if( prmsWidget->getStopValue() != prmsWidget->getStartValue() )
    {
        unsigned int nrsteps = static_cast<unsigned int>( ( prmsWidget->getStopValue() - prmsWidget->getStartValue() ) / prmsWidget->getStepValue() );   
    
        for(unsigned int stepper = 0; stepper <= nrsteps; ++stepper)
        {
            prmsWidget->setAdvancedValue( prmsWidget->getStartValue() + static_cast<double>(prmsWidget->getStepValue() * stepper) );
            {
                QEventLoop pause;
                connect(this, &BaseMCWidget::serverReturn, &pause, &QEventLoop::quit);
                equilibrateAction();
                pause.exec();
            }
            {
                QEventLoop pause;
                connect(this, &BaseMCWidget::serverReturn, &pause, &QEventLoop::quit);
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



DefaultMCWidget::~DefaultMCWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}


    
