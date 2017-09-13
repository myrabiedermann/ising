#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

#include "global.hpp"
#include "long_qspinbox.hpp"
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QtDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QSize>
#include <limits>



class ParametersWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParametersWidget(QWidget *parent = Q_NULLPTR);
    ParametersWidget(const ParametersWidget&) = delete;
    void operator=(const ParametersWidget&) = delete;
    
    ~ParametersWidget();
    
    float getInteraction() const;
    float getMagnetic() const;
    float getTemperature() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    unsigned int getPrintFreq() const;
    unsigned long getSteps() const;
    
    bool getConstrained() const;
    bool getPrint() const;
    
public slots:
    void setReadOnly(bool);
    
signals:
    void valueChanged();
    void criticalValueChanged();
    
protected:
    void applyValues();
    QGroupBox* createSystemBox();
    QGroupBox* createOutputBox();
    QGroupBox* createMCBox();
    
private:
    // Line edits
    QDoubleSpinBox* interactionSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* magneticSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* temperatureSpinBox = new QDoubleSpinBox(this);
    QSpinBox* heightSpinBox = new QSpinBox(this);
    QSpinBox* widthSpinBox = new QSpinBox(this);
    QSpinBox* printFreqSpinBox = new QSpinBox(this);
    QtLongLongSpinBox* stepsSpinBox = new QtLongLongSpinBox(this);
    
    // Buttons
    QPushButton* applyBtn = new QPushButton("Apply",this);
    QCheckBox* constrainedBox = new QCheckBox("Constrained", this);
    QCheckBox* printBox = new QCheckBox("Save trajectory", this);
};
