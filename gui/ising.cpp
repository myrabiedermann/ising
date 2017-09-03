#include "ising.hpp"
#include "ui_ising.h"

ising::ising(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ising)
{
    ui->setupUi(this);
    
    // the layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // ParameterWidget
    ParametersWidget* prmsWidget = new ParametersWidget(this);
    
    // GridWidget
    GridWidget* gridWidget = new GridWidget(this);
    
    // bottom buttons
    QGroupBox* bottomBtns = createBottomActionGroup();
    
    // adding the widgets
    mainLayout->addWidget(prmsWidget);
    mainLayout->addWidget(gridWidget);
    mainLayout->addWidget(bottomBtns);
    
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mainLayout);
    adjustSize();
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QApplication::desktop()->availableGeometry()));
}



QGroupBox* ising::createBottomActionGroup()
{
    // new group
    QGroupBox *groupBox = new QGroupBox();
    
    //TODO change to pause when klicked once
    // the run button 
    QPushButton *runBtn = new QPushButton(tr("&Run"));
    // checked when clicked
    runBtn->setCheckable(true);
    runBtn->setChecked(false);
    runBtn->setMaximumWidth(100);
    //connect button to function
    connect(runBtn, &QPushButton::clicked, qApp, &dummy);
    
    // the quit button
    QPushButton *quitBtn = new QPushButton(tr("&Quit"));
    quitBtn->setMaximumWidth(100);
    //connect button to function
    connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
    
    // pack buttons into layout
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(runBtn);
//     hbox->addStretch(-1);
    hbox->addWidget(quitBtn);
    
    // set layout of group
    groupBox->setLayout(hbox);
    
    return groupBox;
}



ising::~ising()
{
    delete ui;
}
