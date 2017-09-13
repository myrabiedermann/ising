#include "chart_widget.hpp"



ChartWidget::ChartWidget(QWidget *parent)
  : QtCharts::QChartView(parent)
  , chart(new QtCharts::QChart())
  , series(new QtCharts::QLineSeries())
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(series);
    Q_CHECK_PTR(chart);
    
    setChart(chart);
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("DEFAULT TEXT");
    QPen green(Qt::red);
    series->setPen(green);
//     resize(300, 500);
//     fitInView(QRectF(0, 0, 300, 500));
    setRenderHint(QPainter::Antialiasing);
}



ChartWidget::~ChartWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}



void ChartWidget::append(qreal x, qreal y)
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_CHECK_PTR(series);
    qInfo() <<  x <<  "  " <<  y;
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
//     chart->setAxisX(series->getA, series);
}




void ChartWidget::setTitle(const QString& s)
{
    Q_CHECK_PTR(chart);
    chart->setTitle(s);
}



void ChartWidget::reset()
{
    Q_CHECK_PTR(chart);
    series->clear();
}
