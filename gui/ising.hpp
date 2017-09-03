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
    class ising;
}

class ising : public QMainWindow
{
    Q_OBJECT

public:
    explicit ising(QWidget *parent = nullptr);
    ising(const ising&) = delete;
    void operator=(const ising&) = delete;
    ~ising();

private:
    QGroupBox* createBottomActionGroup();
    static void dummy() { std::cout << "Dummy button" << std::endl; }
    
    Ui::ising *ui;
};

#endif // ISING_H
