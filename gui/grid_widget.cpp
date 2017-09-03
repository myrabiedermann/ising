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
    
    
    for(SIZE w_ = 0; w_ < width; ++w_)
    for(SIZE h_ = 0; h_ < height; ++h_)
    {
        Rectangular* rect = new Rectangular;
        rect->setPos(w_,h_);
        scene->addItem(rect);
    }
    
}

