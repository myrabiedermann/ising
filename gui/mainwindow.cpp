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
    runBtn->setCheckable(true);
    runBtn->setMaximumWidth(100);
    connect(runBtn, &QPushButton::toggled, this, &MainWindow::runAction);
    
    // the quit button
    quitBtn->setMaximumWidth(100);
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
    delete prmsWidget;
    delete runBtn;
    delete quitBtn;
}



void MainWindow::runAction()
{
    std::cout << __func__ <<  std::endl;
    
//     runBtn->setChecked(true);

    int i = 0;
    prmsWidget->setReadOnly(true);
    gridWidget->setRowsColumns(prmsWidget->getHeight(), prmsWidget->getWidth());
    
    while ( i++ < prmsWidget->getSteps() && runBtn->isChecked() )
    {
        auto future = QtConcurrent::run([&]
        {
            usleep(1);
        });
        future.waitForFinished();
        
        if (i % prmsWidget->getPrintFreq() == 0)
        {
            gridWidget->draw_test(); 
            gridWidget->refresh();
        }
    }
    prmsWidget->setReadOnly(false);
}
