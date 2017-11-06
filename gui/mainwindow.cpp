#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    defaultLayoutWidget(new DefaultLayoutWidget(this)),
    ui(new Ui::MainWindow)
{
    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(defaultLayoutWidget);
    
    ui->setupUi(this);
    
    // ### the layout
    {    
        QVBoxLayout* mainLayout = new QVBoxLayout;
        Q_CHECK_PTR(mainLayout);

        mainLayout->addWidget(defaultLayoutWidget);
        
        // getting rid of unnecessary empty toolbar
        Q_FOREACH(QToolBar *tb, findChildren<QToolBar*>()) removeToolBar(tb);
        
        centralWidget()->setLayout(mainLayout);
        adjustSize();
        setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QApplication::desktop()->availableGeometry()));
    }

}



MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
}
