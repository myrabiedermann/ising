#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    gridWidget(new GridWidget(this)),
    prmsWidget(new ParametersWidget(this)),
    MCwidget(new MCWidget(this)),
    hamiltonianChart(new ChartWidget()),
    averageSpinChart(new ChartWidget()),
    progressBar(new QProgressBar(this)),
    ui(new Ui::MainWindow)
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(gridWidget);
    Q_CHECK_PTR(prmsWidget);
    Q_CHECK_PTR(MCwidget);
    Q_CHECK_PTR(progressBar);
    
    ui->setupUi(this);
    
    // ### GridWidget
    {   
        gridWidget->setFixedSize(500, 500);
        gridWidget->setSceneRect(0, 0, 500, 500);
        gridWidget->fitInView(0, 0, 500, 500, Qt::KeepAspectRatio);
        gridWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gridWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
        connect( MCwidget, &MCWidget::drawRequest, [&](const MonteCarloHost& system)
        {
            gridWidget->draw(system); 
            gridWidget->refresh(); 
        });
    }
    
    // ### ParametersWidget
    {
        connect( MCwidget, &MCWidget::runningSignal, prmsWidget, &ParametersWidget::setReadOnly );
        connect( MCwidget, &MCWidget::resetSignal, prmsWidget, &ParametersWidget::setDefault );
    }
    
    // ### MCWidget
    {
        MCwidget->setParameters(prmsWidget);
        connect( prmsWidget, &ParametersWidget::criticalValueChanged, MCwidget, &MCWidget::makeSystemNew );
    }
    
    // ### progressBar
    {
        progressBar->reset();
        progressBar->setMinimum(0);
        progressBar->setMaximum(0);
        progressBar->setValue(0);
        
        connect( MCwidget, &MCWidget::resetSignal, progressBar, &QProgressBar::reset );
        connect( MCwidget, &MCWidget::finishedSteps, [&](unsigned long steps_done)
        {
            progressBar->setMaximum(prmsWidget->getSteps()/1000000);
            progressBar->setValue(steps_done/1000000);
        });
    }
    
    // ### hamiltonianChart
    {
        hamiltonianChart->setTitle("Hamiltonian");
        hamiltonianChart->setMinimumHeight(200);
        hamiltonianChart->setMinimumWidth(700);
        
        connect( prmsWidget, &ParametersWidget::criticalValueChanged, hamiltonianChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::resetSignal, hamiltonianChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
        {
            hamiltonianChart->draw(steps, system.getSpinsystem().getHamiltonian());
        });
    }
    
    // ### averageSpinChart
    {
        averageSpinChart->setTitle("Average Spin");
        averageSpinChart->setMinimumHeight(200);
        averageSpinChart->setMinimumWidth(700);
        
        connect( prmsWidget, &ParametersWidget::criticalValueChanged, averageSpinChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::resetSignal, averageSpinChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
        {
            averageSpinChart->draw
            (
                steps, 
                (double)(  
                    system.getSpinsystem().num<SPINTYPE::UP>()-
                    system.getSpinsystem().num<SPINTYPE::DOWN>()
                )/prmsWidget->getWidth()/prmsWidget->getHeight() 
            );
        });
    }
    
    // ### the layout
    {    
        QVBoxLayout* mainLayout = new QVBoxLayout;
        QHBoxLayout* centralAreaLayout = new QHBoxLayout;
        QVBoxLayout* chartLayout = new QVBoxLayout;
        Q_CHECK_PTR(mainLayout);
        Q_CHECK_PTR(centralAreaLayout);
        
        // bottom buttons
        QGroupBox* bottomBtns = createBottomActionGroup();
        Q_CHECK_PTR(bottomBtns);
        
        // the charts
        chartLayout->addWidget(hamiltonianChart);
        chartLayout->addWidget(averageSpinChart);
        
        // central area from left to right
        centralAreaLayout->addWidget(prmsWidget);
        centralAreaLayout->addWidget(gridWidget);
        centralAreaLayout->addLayout(chartLayout);
        
        // complete window from top to bottom
        mainLayout->addLayout(centralAreaLayout);
        mainLayout->addWidget(progressBar);
        mainLayout->addWidget(bottomBtns);
        
        // getting rid of unnecessary empty toolbar
        Q_FOREACH(QToolBar *tb, findChildren<QToolBar*>()) removeToolBar(tb);
        
        centralWidget()->setLayout(mainLayout);
        adjustSize();
        setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QApplication::desktop()->availableGeometry()));
    }
}



QGroupBox* MainWindow::createBottomActionGroup()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(quitBtn);
    
    // new group
    QGroupBox *groupBox = new QGroupBox();
    Q_CHECK_PTR(groupBox);
    
    // the quit button
    quitBtn->setMaximumWidth(100);
    connect(quitBtn, &QPushButton::clicked, QCoreApplication::instance(), &QApplication::quit);
    
    // pack buttons into layout
    QHBoxLayout *hbox = new QHBoxLayout;
    Q_CHECK_PTR(hbox);
    hbox->addWidget(MCwidget);
    hbox->addWidget(quitBtn);
    
    // set layout of group
    groupBox->setLayout(hbox);
    
    return groupBox;
}



MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
}
