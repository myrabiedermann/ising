#include "rectangular.hpp"

Rectangular::Rectangular(QGraphicsItem* parent __attribute__((unused)) )
//   : QGraphicsItem(parent)
{
    // draw
    setRect(0,0,10,10);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::cyan);
    setBrush(brush);
}

Rectangular::Rectangular::~Rectangular()
{
}

