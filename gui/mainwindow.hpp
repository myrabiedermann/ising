#ifndef ISING_H
#define ISING_H


#include "parameters_widget.hpp"
#include "grid_widget.hpp"
#include <QMainWindow>
#include <QtWidgets>
#include <QPushButton>
#include <QGroupBox>
#include <iostream>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow&) = delete;
    void operator=(const MainWindow&) = delete;
    ~MainWindow();
    
    void runAction();

protected:
    QGroupBox* createBottomActionGroup();
    
private:
    static void dummy() { std::cout << "Dummy button" << std::endl; }
    
    GridWidget* gridWidget = new GridWidget(this);
    Ui::MainWindow* ui;
};

#endif // ISING_H
