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
    height_of_rectangular = std::trunc( scene_height / rows );
    width_of_rectangular = std::trunc( scene_width / columns );
}



void GridWidget::draw_test()
{
    std::cout << __PRETTY_FUNCTION__ << columns <<"   "<< rows<< std::endl;
    scene->clear();
    
    for(std::uint16_t column = 0; column < columns; ++column)
    for(std::uint16_t row = 0; row < rows; ++row)
    {
        draw_rectangle(column*width_of_rectangular, row*height_of_rectangular, width_of_rectangular, height_of_rectangular, QPen(Qt::transparent), get_color_of_spin( Spin(0, SPINTYPE(rand()%2*2)) ));
    }
    
//     std::cout << "new scene generated" <<  std::endl;
}



void GridWidget::refresh()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    repaint();
    viewport()->update();
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




void GridWidget::draw( const mc& MC )
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    scene->clear();
    
    for(std::uint16_t column = 0; column < MC.getSpinsystem().getWidth(); ++column)
    for(std::uint16_t row = 0; row < MC.getSpinsystem().getHeight(); ++row)
    {
//         std::cout << "col" << column << std::endl;
//         std::cout << "row" << row << std::endl;
        draw_rectangle(column*width_of_rectangular, row*height_of_rectangular, width_of_rectangular, height_of_rectangular, QPen(Qt::transparent), get_color_of_spin( MC.getSpinsystem().get_spins().at(column*row + column) ));
    }
//     refresh();
}
