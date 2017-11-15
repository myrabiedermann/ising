#include "chart_widget.hpp"



ChartWidget::ChartWidget(QWidget *parent)
  : QtCharts::QChartView(parent)
  , chart(new QtCharts::QChart())
  , series(new QtCharts::QLineSeries())
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(series);
    Q_CHECK_PTR(chart);
    
    chart->setTheme(QtCharts::QChart::ChartThemeQt);
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    // chart->setTitle("DEFAULT TEXT");
    
    chart->axisX()->setTitleText(xLabel);
    chart->axisY()->setTitleText(yLabel);
    
    setChart(chart);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setRenderHint(QPainter::Antialiasing);
    
}



ChartWidget::~ChartWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}



void ChartWidget::append(qreal x, qreal y)
{
    qDebug() << __PRETTY_FUNCTION__ <<  x <<  "  " <<  y;
    Q_CHECK_PTR(series);
    series->append(x, y);
    
    if( first_range_setup ) 
    {
        xMin = xMax = x;
        yMin = yMax = y;
        first_range_setup = false;
    }
    else
    {
        if( x < xMin ) xMin = x;
        if( x > xMax ) xMax = x;
        if( y < yMin ) yMin = y;
        if( y > yMax ) yMax = y;
    }
}



void ChartWidget::draw(qreal x, qreal y)
{
    qDebug() << __PRETTY_FUNCTION__;
    append(x, y);
    refresh();
}



void ChartWidget::refresh()
{
    qDebug() << __PRETTY_FUNCTION__;
    chart->axisX()->setRange(xMin,xMax);
    chart->axisY()->setRange(yMin,yMax);
    repaint();
}



void ChartWidget::setTitle(const QString& s)
{    
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(chart);
    chart->setTitle(s);
}


void ChartWidget::setXLabel(const QString& s)
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(chart);
    chart->axisX()->setTitleText(s);
    xLabel = s;
}


void ChartWidget::setYLabel(const QString& s)
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(chart);
    chart->axisY()->setTitleText(s);
    yLabel = s;
}


void ChartWidget::reset()
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(chart);
    series->clear();
    first_range_setup = true;
}


void ChartWidget::setPen(const QPen& pen)
{
    qDebug() << __PRETTY_FUNCTION__;
    series->setPen(pen);
}
