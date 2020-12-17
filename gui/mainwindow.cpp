#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    gridWidget(new GridWidget(this)),
    prmsWidget(Q_NULLPTR),
    mcWidget(Q_NULLPTR),
    hamiltonianChart(new ChartWidget(this)),
    averageMagnetisationChart(Q_NULLPTR),
    correlationChart(Q_NULLPTR),
    ui(new Ui::MainWindow)
{
    qDebug() << __PRETTY_FUNCTION__;

    ui->setupUi(this);
    
    // ### ask user for systemType
    MessageBox msgBox;
    msgBox.exec();
    
    // ### create layout according to systemType
    switch( msgBox.getType() )
    {
        case SYSTEMTYPE::Default :      prmsWidget = new DefaultParametersWidget(this); 
                                        mcWidget = new DefaultMCWidget(this);
                                        isingLOG("gui: " << "running in spin-flip mode")
                                        break;
        
        case SYSTEMTYPE::Constrained :  prmsWidget = new ConstrainedParametersWidget(this);
                                        mcWidget = new ConstrainedMCWidget(this);
                                        isingLOG("gui: " << "running in spin-exchange mode")
                                        break; 

        default :                       throw std::logic_error("nope");
    }


    // ### GridWidget
    {   
        gridWidget->setFixedSize(500, 500);
        gridWidget->setSceneRect(0, 0, 500, 500);
        gridWidget->fitInView(0, 0, 500, 500, Qt::KeepAspectRatio);
        gridWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gridWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        
        connect( mcWidget, &BaseMCWidget::drawRequest, [&](const MonteCarloHost& system)
        {
            gridWidget->draw(system); 
            gridWidget->refresh(); 
        });
    }
    
    // ### ParametersWidget
    {
        prmsWidget->setMinimumWidth(370);
        prmsWidget->setMaximumWidth(500);
        prmsWidget->setMinimumHeight(600);
        prmsWidget->setMaximumHeight(700);
        connect( mcWidget, &BaseMCWidget::runningSignal, prmsWidget, &BaseParametersWidget::setReadOnly );
        connect( mcWidget, &BaseMCWidget::resetSignal, prmsWidget, &BaseParametersWidget::setDefault );
    }
    
    // ### MCWidget
    {
        mcWidget->setParameters(prmsWidget);
        connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, mcWidget, &BaseMCWidget::makeSystemNew );
        connect( prmsWidget, &BaseParametersWidget::valueChanged, mcWidget, &BaseMCWidget::makeRecordsNew );
        connect( prmsWidget, &BaseParametersWidget::randomise, mcWidget, &BaseMCWidget::makeSystemRandom);
    }
    
    // ### upper chart: hamiltonianChart
    {
        hamiltonianChart->setXLabel("MC steps");
        hamiltonianChart->setYLabel("Hamiltonian");
        hamiltonianChart->setMinimumHeight(300);
        hamiltonianChart->setMinimumWidth(480);
        
        connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, hamiltonianChart, &ChartWidget::reset );
        connect( mcWidget, &BaseMCWidget::resetChartSignal, hamiltonianChart, &ChartWidget::reset);
        connect( mcWidget, &BaseMCWidget::resetSignal, hamiltonianChart, &ChartWidget::reset );
        connect( mcWidget, &BaseMCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
        {
            hamiltonianChart->draw(steps, system.getSpinsystem().getHamiltonian());
        });
    }
    
    // ### lower chart: averageMagnetisationChart / correlationChart
    {
        if( ! prmsWidget->getConstrained() )
        {
            averageMagnetisationChart = new ChartWidget(this);
            Q_CHECK_PTR(averageMagnetisationChart);

            averageMagnetisationChart->setXLabel("MC steps");
            averageMagnetisationChart->setYLabel("Magnetisation per spin");
            averageMagnetisationChart->setMinimumHeight(300);
            averageMagnetisationChart->setMinimumWidth(480);
            
            connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, averageMagnetisationChart, &ChartWidget::reset );
            connect( mcWidget, &BaseMCWidget::resetChartSignal, averageMagnetisationChart, &ChartWidget::reset);
            connect( mcWidget, &BaseMCWidget::resetSignal, averageMagnetisationChart, &ChartWidget::reset );
            connect( mcWidget, &BaseMCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
            {
                averageMagnetisationChart->draw(steps, system.getSpinsystem().getMagnetisation());
            });
        }
        else
        {
            correlationChart = new ChartWidget(this);
            Q_CHECK_PTR(correlationChart);

            correlationChart->setXLabel("distance");
            correlationChart->setYLabel("correlation");
            correlationChart->setMinimumHeight(300);
            correlationChart->setMinimumWidth(480);

            connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, correlationChart, &ChartWidget::reset );
            connect( mcWidget, &BaseMCWidget::resetChartSignal, correlationChart, &ChartWidget::reset);
            connect( mcWidget, &BaseMCWidget::resetSignal, correlationChart, &ChartWidget::reset );
            connect( mcWidget, &BaseMCWidget::drawCorrelationRequest, [&](const Histogram<double>& correlation)
            {
                for(const auto& B : correlation )
                {
                    correlationChart->append(B.position(), B.counter);
                }
                correlationChart->refresh();
            });
        }
    }
    
    // ### the layout
    {    
        QVBoxLayout* mainLayout = new QVBoxLayout;
        QHBoxLayout* CentralAreaLayout = new QHBoxLayout;
        QVBoxLayout* chartLayout = new QVBoxLayout;
        Q_CHECK_PTR(mainLayout);
        Q_CHECK_PTR(CentralAreaLayout);
        Q_CHECK_PTR(chartLayout);
        
        // the charts
        chartLayout->addWidget(hamiltonianChart);
        if( ! prmsWidget->getConstrained() )
        {
            chartLayout->addWidget(averageMagnetisationChart);
        }
        else
        {
            chartLayout->addWidget(correlationChart);
        }
        
        // central area from left to right
        CentralAreaLayout->addWidget(prmsWidget);
        CentralAreaLayout->addWidget(gridWidget);
        CentralAreaLayout->addLayout(chartLayout);

        // complete window from top to bottom
        mainLayout->addLayout(CentralAreaLayout);
        mainLayout->addWidget(createBottomActionGroup());
        
        // getting rid of unnecessary empty toolbar
        Q_FOREACH(QToolBar *tb, findChildren<QToolBar*>()) removeToolBar(tb);
        centralWidget()->setLayout(mainLayout);
    }    
    
    adjustSize();
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QApplication::desktop()->availableGeometry()));

}



QGroupBox* MainWindow::createBottomActionGroup()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(quitBtn);
    Q_CHECK_PTR(mcWidget);
    
    // new group
    QGroupBox *groupBox = new QGroupBox();
    Q_CHECK_PTR(groupBox);
    
    // the quit button
    quitBtn->setMinimumWidth(40);
    quitBtn->setMaximumWidth(80);
    quitBtn->setMinimumHeight(25);
    quitBtn->setMaximumHeight(30);
    connect(quitBtn, &QPushButton::clicked, this, &MainWindow::quitAction);
    
    // pack buttons into layout
    QHBoxLayout *hbox = new QHBoxLayout;
    Q_CHECK_PTR(hbox);
    hbox->addWidget(mcWidget);
    hbox->addWidget(quitBtn);
    
    // set layout of group
    groupBox->setLayout(hbox);

    groupBox->setMinimumHeight(80);
    groupBox->setMaximumHeight(100);
    groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    return groupBox;
}


void MainWindow::quitAction()
{
    qDebug() << __PRETTY_FUNCTION__;

    isingLOG("- - - end of program - - -")
    QApplication::quit();
}



MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
}
