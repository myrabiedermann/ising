#include "grid_widget.hpp"



GridWidget::GridWidget(QWidget *parent) 
  : QGraphicsView(parent)
  , scene( new QGraphicsScene(0,0,scene_width,scene_height))
{
    setScene(scene);
}



GridWidget::~GridWidget()
{
    delete scene;
}



void GridWidget::showEvent(QShowEvent *) 
{
    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}



void GridWidget::make_new_scene()
{
    QGraphicsScene* scene_temp = scene;
    scene = new QGraphicsScene(0,0,scene_width,scene_height);
    delete scene_temp;
    setScene(scene);
}





void GridWidget::setRowsColumns(std::uint16_t r, std::uint16_t c)
{
    rows = r;
    columns = c;
    width_of_rectangular = std::trunc( scene_width / columns );
    height_of_rectangular = std::trunc( scene_height / rows );
}



void GridWidget::draw_test()
{
    make_new_scene();
    
    for(std::uint16_t column = 0; column < columns; ++column)
    for(std::uint16_t row = 0; row < rows; ++row)
    {
        draw_rectangle(column*width_of_rectangular, row*height_of_rectangular, width_of_rectangular, height_of_rectangular, QPen(Qt::black), get_color_of_spin( Spin(0, SPINTYPE(rand()%2*2)) ));
    }
}



void GridWidget::setColumns(std::uint16_t c)
{
    columns = c;
    width_of_rectangular = std::trunc( scene_width / columns );
}


void GridWidget::setRows(std::uint16_t r)
{
    rows = r;
    height_of_rectangular = std::trunc( scene_height / rows );
}



void GridWidget::draw_rectangle(std::uint16_t x, std::uint16_t y, std::uint16_t dx , std::uint16_t dy, QPen border, QBrush fill)
{
    scene->addRect(x, y, dx, dy, border, fill);
}



QBrush GridWidget::get_color_of_spin(const Spin& spin)
{
    switch (spin.get_type())
    {
        case SPINTYPE::UP : return QBrush(Qt::red);
        case SPINTYPE::DOWN : return QBrush(Qt::yellow);
        default : return QBrush(Qt::black);
    }
}




void GridWidget::draw( const Spinsystem& spinsystem )
{
    make_new_scene();
    const auto edge = spinsystem.get_edge();
    
    for(std::uint16_t column = 0; column < edge; ++column)
    for(std::uint16_t row = 0; row < edge; ++row)
    {
        draw_rectangle(column*width_of_rectangular, row*height_of_rectangular, width_of_rectangular, height_of_rectangular, QPen(Qt::black), get_color_of_spin( spinsystem.get_spins().at(column*row + column) ));
    }
    
}
