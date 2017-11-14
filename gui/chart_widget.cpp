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
    // repaint();
    chart->removeSeries(series);
    chart->addSeries( series );
    // chart->removeAxis(chart->axisX());
    // chart->removeAxis(chart->axisY());
    chart->createDefaultAxes();
    // chart->setAxisX(chart->axisX(), series);
    // chart->setAxisY(chart->axisY(), series);
    // series->attachAxis(chart->axisX());
    chart->axisX()->setTitleText(xLabel);
    chart->axisY()->setTitleText(yLabel);
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
    // chart->axes().clear();
}


void ChartWidget::setPen(const QPen& pen)
{
    qDebug() << __PRETTY_FUNCTION__;
    series->setPen(pen);
}
