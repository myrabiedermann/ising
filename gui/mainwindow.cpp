#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // the layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* columnLayout = new QHBoxLayout;
    
    // ParameterWidget
    ParametersWidget* prmsWidget = new ParametersWidget(this);
    
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
    
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mainLayout);
    adjustSize();
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QApplication::desktop()->availableGeometry()));
}



QGroupBox* MainWindow::createBottomActionGroup()
{
    // new group
    QGroupBox *groupBox = new QGroupBox();
    
    //TODO change to pause when klicked once
    // the run button 
    QPushButton *runBtn = new QPushButton("Run",this);
    // checked when clicked
    runBtn->setCheckable(true);
    runBtn->setChecked(false);
    runBtn->setMaximumWidth(100);
    //connect button to function
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runAction);
    
    // the quit button
    QPushButton *quitBtn = new QPushButton("Quit",this);
    runBtn->setCheckable(false);
    runBtn->setChecked(false);
    quitBtn->setMaximumWidth(100);
    //connect button to function
    connect(quitBtn, &QPushButton::clicked, QCoreApplication::instance(), &QApplication::quit);
    
    // pack buttons into layout
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(runBtn);
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
}



void MainWindow::runAction()
{
    gridWidget->setRowsColumns(20, 20);
    gridWidget->draw_test();
}
