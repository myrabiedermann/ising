#ifndef ISING_H
#define ISING_H

#include <QMainWindow>
#include <QtWidgets>
#include <QPushButton>
#include <QGroupBox>
#include <iostream>


namespace Ui {
    class ising;
}

class ising : public QMainWindow
{
    Q_OBJECT

public:
    explicit ising(QWidget *parent = 0);
    ~ising();

private:
    QGroupBox* createBottomActionGroup();
    static const void dummy() { std::cout << "Dummy button" << std::endl; }
    
    Ui::ising *ui;
};

#endif // ISING_H
