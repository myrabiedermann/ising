#include "grid_widget.hpp"



GridWidget::GridWidget(QWidget *parent) 
  : QGraphicsView(parent)
  , scene( new QGraphicsScene(0,0,scene_width,scene_height))
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(scene);
    setScene(scene);
}



GridWidget::~GridWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}



void GridWidget::showEvent(QShowEvent *) 
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(scene);
    fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}



void GridWidget::makeNewScene()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(scene);
    QGraphicsScene* scene_temp = scene;
    Q_CHECK_PTR(scene_temp);
    scene = new QGraphicsScene(0,0,scene_width,scene_height);
    delete scene_temp;
    Q_CHECK_PTR(!scene_temp);
    Q_CHECK_PTR(scene);
    setScene(scene);
}




void GridWidget::setRowsColumns(unsigned short r, unsigned short c)
{
    qDebug() << __PRETTY_FUNCTION__;
    rows = r;
    columns = c;
    height_of_rectangular = std::trunc( scene_height / rows );
    width_of_rectangular = std::trunc( scene_width / columns );
}




void GridWidget::refresh()
{
    qDebug() << __PRETTY_FUNCTION__;
    repaint();
    viewport()->update();
}




void GridWidget::setColumns(unsigned short c)
{
    qDebug() << __PRETTY_FUNCTION__;
    columns = c;
    width_of_rectangular = std::trunc( scene_width / columns );
}


void GridWidget::setRows(unsigned short r)
{
    qDebug() << __PRETTY_FUNCTION__;
    rows = r;
    height_of_rectangular = std::trunc( scene_height / rows );
}



void GridWidget::drawRectangle(unsigned short x, unsigned short y, unsigned short dx , unsigned short dy, QPen border, QBrush fill)
{
    Q_CHECK_PTR(scene);
    scene->addRect(x, y, dx, dy, border, fill);
}



QBrush GridWidget::getSpinColor(const Spin& spin)
{
    switch (spin.get_type())
    {
        case SPINTYPE::UP : return QBrush(Qt::red);
        case SPINTYPE::DOWN : return QBrush(Qt::yellow);
        default : return QBrush(Qt::black);
    }
}



void GridWidget::draw_test()
{
    qDebug() << __PRETTY_FUNCTION__ << columns <<"   "<< rows;
    Q_CHECK_PTR(scene);
    
    scene->clear();
    
    for(unsigned short column = 0; column < columns; ++column)
    for(unsigned short row = 0; row < rows; ++row)
    {
            drawRectangle
        (
            column*width_of_rectangular, 
            row*height_of_rectangular, 
            width_of_rectangular, 
            height_of_rectangular, 
            QPen(Qt::transparent), 
                getSpinColor( Spin(0, SPINTYPE(rand()%2*2)) 
        ));
    }
}



void GridWidget::draw( const MonteCarloHost& system )
{
    qDebug() << __PRETTY_FUNCTION__;
    
    draw( system.getSpinsystem() );
}



void GridWidget::draw( const Spinsystem& system )
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(scene);
    
    scene->clear();
    
    setColumns(system.getWidth());
    setRows(system.getHeight());
    
    for(unsigned short row = 0; row < rows; ++row)
    for(unsigned short column = 0; column < columns; ++column)
    {
            drawRectangle
        ( 
            column*width_of_rectangular, 
            row*height_of_rectangular, 
            width_of_rectangular, 
            height_of_rectangular, 
            QPen(Qt::transparent), 
                getSpinColor( system.getSpins().at(columns*row + column) 
        ));
    }
}
