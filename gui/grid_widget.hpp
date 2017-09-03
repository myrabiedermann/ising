#pragma once

#include "global.hpp"
#include "rectangular.hpp"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QGridLayout>
#include <QRect>


class GridWidget : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit GridWidget(QWidget *parent = nullptr);
    GridWidget(const GridWidget&) = delete;
    void operator=(const GridWidget&) = delete;
    
    ~GridWidget();
    
    void setWidthHeight(SIZE,SIZE);
    
protected:
    
private:
    SIZE height = 0;
    SIZE width = 0;
    
    QGraphicsScene* scene = new QGraphicsScene(0,0,500,500);
};
