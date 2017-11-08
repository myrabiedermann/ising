#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

#include "mainwindow.hpp"
#include <QtWidgets>
#include <QPushButton>
#include <QToolBar>
#include <QtDebug>
#include <QGroupBox>
#include <QTimer>
#include <QProgressBar>
#include <iostream>
#include <cstdint>


enum SYSTEMTYPE{ Default, Constrained };


class MessageBox : public QMessageBox
{
    Q_OBJECT

public:
    explicit MessageBox(QWidget* parent = Q_NULLPTR);
    MessageBox(const MessageBox&) = delete;
    void operator=(const MessageBox&) = delete;
    
    ~MessageBox();

    SYSTEMTYPE getType() const;

protected:

private:
    QPushButton *defaultButton;
    QPushButton *constrainedButton;

};