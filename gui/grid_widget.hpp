#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

#ifndef USE_MATH_DEFINES
    #define USE_MATH_DEFINES
#endif


// #include "global.hpp"
#include "system/montecarlohost.hpp"
#include "system/spinsystem.hpp"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QGridLayout>
#include <QRect>
#include <QtDebug>
#include <QShowEvent>
#include <cmath>
#include <type_traits>
#include <cstdint>
#include <iostream>


class GridWidget : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit GridWidget(QWidget *parent = Q_NULLPTR);
    GridWidget(const GridWidget&) = delete;
    void operator=(const GridWidget&) = delete;
    
    ~GridWidget();
    
    void showEvent(QShowEvent *);
    
    void setRowsColumns(unsigned short,unsigned short);
    void setRows(unsigned short);
    void setColumns(unsigned short);
    
public slots:
    void draw(const MonteCarloHost&);
    void draw(const Spinsystem&);
    void draw_test();
    
    void refresh();
    
protected:
    QBrush getSpinColor(const Spin& spin);
    void drawRectangle(unsigned short, unsigned short, unsigned short, unsigned short, QPen, QBrush);
    void makeNewScene();
    
private:
    unsigned short rows = 0;
    unsigned short columns = 0;
    
    unsigned short width_of_rectangular  = 0;
    unsigned short height_of_rectangular = 0;
    
    const unsigned short scene_width = 500;
    const unsigned short scene_height = 500;
    
    QGraphicsScene* scene;
};
