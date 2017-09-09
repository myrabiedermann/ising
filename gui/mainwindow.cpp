#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    gridWidget(new GridWidget(this)),
    prmsWidget(new ParametersWidget(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // the layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* columnLayout = new QHBoxLayout;
    
    // GridWidget
    gridWidget->setFixedSize(500, 500);
    gridWidget->setSceneRect(0, 0, 500, 500);
    gridWidget->fitInView(0, 0, 500, 500, Qt::KeepAspectRatio);
    gridWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // bottom buttons
    QGroupBox* bottomBtns = createBottomActionGroup();
    
    // adding the widgets
    columnLayout->addWidget(prmsWidget);
    columnLayout->addWidget(gridWidget);
    
    mainLayout->addLayout(columnLayout);
    mainLayout->addWidget(bottomBtns);
    
    connect( MCwidget, &MCWidget::runningSignal, prmsWidget, &ParametersWidget::setReadOnly);
    connect( MCwidget, &MCWidget::drawRequest, [&](const auto& a){ std::cout << __PRETTY_FUNCTION__<< std::endl; gridWidget->draw_test(); });
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), gridWidget, SLOT(refresh()));
    timer->start(1000);
//     connect( MCwidget, &MCWidget::drawRequest, gridWidget, &GridWidget::refresh);
    
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mainLayout);
    adjustSize();
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QApplication::desktop()->availableGeometry()));
    
    assert(prmsWidget);
    MCwidget->setParameters(prmsWidget);
}



QGroupBox* MainWindow::createBottomActionGroup()
{
    // new group
    QGroupBox *groupBox = new QGroupBox();
    
    // the quit button
    quitBtn->setMaximumWidth(100);
    connect(quitBtn, &QPushButton::clicked, QCoreApplication::instance(), &QApplication::quit);
    
    // pack buttons into layout
    QHBoxLayout *hbox = new QHBoxLayout;
//     hbox->addWidget(runBtn);
    hbox->addWidget(MCwidget);
//     hbox->addStretch(-1);
    hbox->addWidget(quitBtn);
    
    // set layout of group
    groupBox->setLayout(hbox);
    
    return groupBox;
}



MainWindow::~MainWindow()
{
    delete ui;
    delete gridWidget;
    delete prmsWidget;
//     delete runBtn;
    delete MCwidget;
    delete quitBtn;
}
