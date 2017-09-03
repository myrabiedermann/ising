#pragma once

#include <QGraphicsRectItem>
#include <QBrush>


class Rectangular: public QGraphicsRectItem
{
//     Q_OBJECT
    
public:
    Rectangular(QGraphicsItem* parent=nullptr);
    Rectangular(const Rectangular&) = delete;
    void operator=(const Rectangular&) = delete;
    ~Rectangular();
};
