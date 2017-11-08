#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    LayoutWidget(Q_NULLPTR),
    ui(new Ui::MainWindow)
{
    qDebug() << __PRETTY_FUNCTION__;

    
    ui->setupUi(this);
    
    // ### the layout
    {    
        QVBoxLayout* mainLayout = new QVBoxLayout;
        Q_CHECK_PTR(mainLayout);
        
        // ask user for systemType
        MessageBox msgBox;
        msgBox.exec();

        // create layout according to systemType
        switch( msgBox.getType() )
        {
            case SYSTEMTYPE::Default :      LayoutWidget = new DefaultLayoutWidget(this); break;
            
            case SYSTEMTYPE::Constrained :  LayoutWidget = new ConstrainedLayoutWidget(this); break;
            default :                       throw std::logic_error("nope");
        }
        Q_CHECK_PTR(LayoutWidget);
        mainLayout->addWidget(LayoutWidget);
        
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
