#include "ising.hpp"
#include "ui_ising.h"

ising::ising(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ising)
{
    ui->setupUi(this);
    
    // the layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // bottom buttons
    QGroupBox* bottomBtns = createBottomActionGroup();
    
    // adding the widgets
    mainLayout->addWidget(bottomBtns);
    
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mainLayout);
    adjustSize();
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QApplication::desktop()->availableGeometry()));
}


QGroupBox* ising::createBottomActionGroup()
{
    QGroupBox *groupBox = new QGroupBox();
    
    QPushButton *runBtn = new QPushButton(tr("&Run"));
    runBtn->setCheckable(true);
    runBtn->setChecked(false);    
    runBtn->setMaximumWidth(100);
    connect(runBtn, &QPushButton::clicked, qApp, &dummy);
    
    QPushButton *quitBtn = new QPushButton(tr("&Quit"));
    quitBtn->setMaximumWidth(100);
    connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
    
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(runBtn);
    //     hbox->addStretch(1);
    hbox->addWidget(quitBtn);
    groupBox->setLayout(hbox);
    
    return groupBox;
}



ising::~ising()
{
    delete ui;
}
