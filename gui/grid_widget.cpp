#include "grid_widget.hpp"



GridWidget::GridWidget(QWidget *parent) 
  : QGraphicsView(parent)
{
    setScene(scene);
}



GridWidget::~GridWidget()
{
}



void GridWidget::setWidthHeight(SIZE w, SIZE h)
{
    width = w;
    height = h;
    
    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QBrush grayBrush(Qt::gray);
    QPen blackPen(Qt::black);
    QPen grayPen(Qt::gray);;
    
    for(SIZE w_ = 0; w_ < width; ++w_)
    for(SIZE h_ = 0; h_ < height; ++h_)
    {
        scene->addRect(w_*10, h_*10, 10, 10, blackPen, grayBrush);
    }
}

