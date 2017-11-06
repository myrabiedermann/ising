#include "default_layout_widget.hpp"

DefaultLayoutWidget::DefaultLayoutWidget(QWidget *parent) :
    QWidget(parent),
    gridWidget(new GridWidget(this)),
    prmsWidget(new ParametersWidget(this)),
    MCwidget(new MCWidget(this)),
    hamiltonianChart(new ChartWidget(this)),
    averageMagnetisationChart(new ChartWidget(this))
    // progressBar(new QProgressBar(this)),
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(gridWidget);
    Q_CHECK_PTR(prmsWidget);
    Q_CHECK_PTR(MCwidget);
    Q_CHECK_PTR(hamiltonianChart);
    Q_CHECK_PTR(averageMagnetisationChart);
    // Q_CHECK_PTR(progressBar);
    
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
        prmsWidget->setMinimumWidth(300);
        connect( MCwidget, &MCWidget::runningSignal, prmsWidget, &ParametersWidget::setReadOnly );
        connect( MCwidget, &MCWidget::resetSignal, prmsWidget, &ParametersWidget::setDefault );
    }
    
    // ### MCWidget
    {
        MCwidget->setParameters(prmsWidget);
        connect( prmsWidget, &ParametersWidget::criticalValueChanged, MCwidget, &MCWidget::makeSystemNew );
        connect( prmsWidget, &ParametersWidget::valueChanged, MCwidget, &MCWidget::makeRecordsNew );
        connect( prmsWidget, &ParametersWidget::randomise, MCwidget, &MCWidget::randomiseSystem );
    }
    
    // ### progressBar
    // {
    //     progressBar->reset();
    //     progressBar->setMinimum(0);
    //     progressBar->setMaximum(0);
    //     progressBar->setValue(0);
        
    //     connect( MCwidget, &MCWidget::resetSignal, progressBar, &QProgressBar::reset );
    //     connect( MCwidget, &MCWidget::finishedSteps, [&](unsigned long steps_done)
    //     {
    //         progressBar->setMaximum(prmsWidget->getSteps()/1000000);
    //         progressBar->setValue(steps_done/1000000);
    //     });
    // }
    
    // ### hamiltonianChart
    {
        hamiltonianChart->setTitle("Hamiltonian");
        hamiltonianChart->setMinimumHeight(400);
        hamiltonianChart->setMinimumWidth(600);
        
        connect( prmsWidget, &ParametersWidget::criticalValueChanged, hamiltonianChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::resetChartSignal, hamiltonianChart, &ChartWidget::reset);
        connect( MCwidget, &MCWidget::resetSignal, hamiltonianChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
        {
            hamiltonianChart->draw(steps, system.getSpinsystem().getHamiltonian());
        });
    }
    
    // ### averageMagnetisationChart
    {
        averageMagnetisationChart->setTitle("average magnetisation per spin");
        averageMagnetisationChart->setMinimumHeight(400);
        averageMagnetisationChart->setMinimumWidth(600);
        
        connect( prmsWidget, &ParametersWidget::criticalValueChanged, averageMagnetisationChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::resetChartSignal, averageMagnetisationChart, &ChartWidget::reset);
        connect( MCwidget, &MCWidget::resetSignal, averageMagnetisationChart, &ChartWidget::reset );
        connect( MCwidget, &MCWidget::drawRequest, [&](const MonteCarloHost& system, const unsigned long steps)
        {
            averageMagnetisationChart->draw(steps, system.getSpinsystem().getMagnetisation());
        });
    }
    
    // ### the layout
    {    
        QVBoxLayout* mainLayout = new QVBoxLayout;
        QHBoxLayout* CentralAreaLayout = new QHBoxLayout;
        QVBoxLayout* chartLayout = new QVBoxLayout;
        Q_CHECK_PTR(mainLayout);
        Q_CHECK_PTR(CentralAreaLayout);
        Q_CHECK_PTR(chartLayout);
        
        // bottom buttons
        QGroupBox* bottomBtns = createBottomActionGroup();
        Q_CHECK_PTR(bottomBtns);
        
        // the charts
        chartLayout->addWidget(hamiltonianChart);
        chartLayout->addWidget(averageMagnetisationChart);
        
        // central area from left to right
        CentralAreaLayout->addWidget(prmsWidget);
        CentralAreaLayout->addWidget(gridWidget);
        CentralAreaLayout->addLayout(chartLayout);

        // complete window from top to bottom
        mainLayout->addLayout(CentralAreaLayout);
        // mainLayout->addWidget(progressBar);
        mainLayout->addWidget(bottomBtns);
        
        setLayout(mainLayout);
    }
}

    

QGroupBox* DefaultLayoutWidget::createBottomActionGroup()
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


DefaultLayoutWidget::~DefaultLayoutWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}