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
    
    setChart(chart);
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
    chart->removeSeries(series);
    chart->addSeries( series );
    chart->createDefaultAxes();
}



void ChartWidget::setTitle(const QString& s)
{
    Q_CHECK_PTR(chart);
    chart->setTitle(s);
}


void ChartWidget::setXLabel(const QString& s)
{
    Q_CHECK_PTR(chart);
    chart->axisX()->setTitleText(s);
}


void ChartWidget::setYLabel(const QString& s)
{
    Q_CHECK_PTR(chart);
    chart->axisY()->setTitleText(s);
}


void ChartWidget::reset()
{
    Q_CHECK_PTR(chart);
    series->clear();
}


void ChartWidget::setPen(const QPen& pen)
{
    series->setPen(pen);
}
