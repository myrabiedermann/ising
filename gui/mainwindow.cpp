#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    gridWidget(new GridWidget(this)),
    prmsWidget(Q_NULLPTR),
    MCwidget(Q_NULLPTR),
    hamiltonianChart(new ChartWidget(this)),
    averageMagnetisationChart(Q_NULLPTR),
    correlationChart(Q_NULLPTR),
    ui(new Ui::MainWindow)
{
    qDebug() << __PRETTY_FUNCTION__;

    
    ui->setupUi(this);
    
    // ask user for systemType
    MessageBox msgBox;
    msgBox.exec();
    
    // create layout according to systemType
    switch( msgBox.getType() )
    {
        case SYSTEMTYPE::Default :      prmsWidget = new DefaultParametersWidget(this); 
                                        MCwidget = new DefaultMCWidget(this);
                                        break;
        
        case SYSTEMTYPE::Constrained :  prmsWidget = new ConstrainedParametersWidget(this);
                                        MCwidget = new ConstrainedMCWidget(this);
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
        
        connect( MCwidget, &BaseMCWidget::drawRequest, [&](const MonteCarloHost& system)
        {
            gridWidget->draw(system); 
            gridWidget->refresh(); 
        });
    }
    
    // ### ParametersWidget
    {
        prmsWidget->setMinimumWidth(300);
        connect( MCwidget, &BaseMCWidget::runningSignal, prmsWidget, &BaseParametersWidget::setReadOnly );
        connect( MCwidget, &BaseMCWidget::resetSignal, prmsWidget, &BaseParametersWidget::setDefault );
    }
    
    // ### MCWidget
    {
        MCwidget->setParameters(prmsWidget);
        connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, MCwidget, &BaseMCWidget::makeSystemNew );
        connect( prmsWidget, &BaseParametersWidget::valueChanged, MCwidget, &BaseMCWidget::makeRecordsNew );
        connect( prmsWidget, &BaseParametersWidget::randomise, MCwidget, &BaseMCWidget::makeSystemRandom);
    }
    
    // ### upper chart: hamiltonianChart
    {
        // hamiltonianChart->setTitle("Hamiltonian");
        hamiltonianChart->setXLabel("MC steps");
        hamiltonianChart->setYLabel("Hamiltonian");
        hamiltonianChart->setMinimumHeight(400);
        hamiltonianChart->setMinimumWidth(600);
        
        connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, hamiltonianChart, &ChartWidget::reset );
        connect( MCwidget, &BaseMCWidget::resetChartSignal, hamiltonianChart, &ChartWidget::reset);
        connect( MCwidget, &BaseMCWidget::resetSignal, hamiltonianChart, &ChartWidget::reset );
        connect( MCwidget, &BaseMCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
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

            // averageMagnetisationChart->setTitle("average magnetisation per spin");
            averageMagnetisationChart->setXLabel("MC steps");
            averageMagnetisationChart->setYLabel("Magnetisation per spin");
            averageMagnetisationChart->setMinimumHeight(400);
            averageMagnetisationChart->setMinimumWidth(600);
            
            connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, averageMagnetisationChart, &ChartWidget::reset );
            connect( MCwidget, &BaseMCWidget::resetChartSignal, averageMagnetisationChart, &ChartWidget::reset);
            connect( MCwidget, &BaseMCWidget::resetSignal, averageMagnetisationChart, &ChartWidget::reset );
            connect( MCwidget, &BaseMCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
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
            correlationChart->setMinimumHeight(400);
            correlationChart->setMinimumWidth(600);

            connect( prmsWidget, &BaseParametersWidget::criticalValueChanged, correlationChart, &ChartWidget::reset );
            connect( MCwidget, &BaseMCWidget::resetChartSignal, correlationChart, &ChartWidget::reset);
            connect( MCwidget, &BaseMCWidget::resetSignal, correlationChart, &ChartWidget::reset );
            connect( MCwidget, &BaseMCWidget::drawCorrelationRequest, [&](const Histogram<double>& correlation)
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
    Q_CHECK_PTR(MCwidget);
    
    // new group
    QGroupBox *groupBox = new QGroupBox();
    Q_CHECK_PTR(groupBox);
    
    // the quit button
    quitBtn->setMaximumWidth(150);
    quitBtn->setMinimumWidth(50);
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
