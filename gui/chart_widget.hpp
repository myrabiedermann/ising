#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include <QDebug>
#include <QPen>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>


class ChartWidget :  public QtCharts::QChartView
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = Q_NULLPTR);
    ChartWidget(const ChartWidget&) = delete;
    void operator=(const ChartWidget&) = delete;
    ~ChartWidget();
    
    
public slots:
    void append(qreal, qreal);
    void draw(qreal, qreal);
    void refresh();
    void setTitle(const QString&);
    void setXLabel(const QString&);
    void setYLabel(const QString&);
    void setPen(const QPen&);
    void reset();
    
signals:
    
    
protected:
    
private:
    QtCharts::QChart* chart;
    QtCharts::QLineSeries* series;
    QString xLabel {};
    QString yLabel {};
    
    bool first_range_setup = true;
    float xMin { 0.0 };
    float xMax { 0.0 };
    float yMin { 0.0 };
    float yMax { 0.0 };
};
