#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

#include "global.hpp"
#include "system/MC.hpp"
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
    
    void setRowsColumns(std::uint16_t,std::uint16_t);
    void setRows(std::uint16_t);
    void setColumns(std::uint16_t);
    
public slots:
    void draw(const mc&);
    void draw(const Spinsystem&);
    void draw_test();
    
    void refresh();
    
protected:
    QBrush get_color_of_spin(const Spin& spin);
    void draw_rectangle(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t, QPen, QBrush);
    void make_new_scene();
    
private:
    std::uint16_t rows = 0;
    std::uint16_t columns = 0;
    
    std::uint16_t width_of_rectangular  = 0;
    std::uint16_t height_of_rectangular = 0;
    
    const std::uint16_t scene_width = 500;
    const std::uint16_t scene_height = 500;
    
    QGraphicsScene* scene;
};
