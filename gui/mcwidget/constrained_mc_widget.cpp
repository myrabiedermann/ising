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
      setup();
}



void ConstrainedMCWidget::setup()
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
 
    CONSTRAINED_MC_WIDGET_ASSERT_ALL;
    
    mainLayout->addWidget(equilBtn);
    mainLayout->addWidget(prodBtn);
    mainLayout->addWidget(pauseBtn);
    mainLayout->addWidget(abortBtn);
    mainLayout->addWidget(saveBtn);
    mainLayout->addWidget(correlateBtn);

    setLayout(mainLayout);  
}



ConstrainedMCWidget::~ConstrainedMCWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}