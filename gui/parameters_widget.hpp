#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

// #include "global.hpp"
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
    
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    double getInteraction() const;
    double getMagnetic() const;
    double getTemperature() const;
    unsigned long getStepsEquil() const;
    unsigned long getStepsProd() const;
    unsigned int getPrintFreq() const;
    
    double getRatio() const;

    void setConstrained(const bool);
    bool getConstrained() const;
    // bool getPrintData() const;
    // bool getPrintAver() const;
    
public slots:
    void setReadOnly(bool);
    void setDefault();
    
signals:
    void valueChanged(); 
    void criticalValueChanged();
    void randomise();
    
protected:
    // // void applyValues();
    void randomiseSystem();
    QGroupBox* createSystemBox();
    QGroupBox* createEquilBox();
    QGroupBox* createProdBox();
    
private:
    bool CONSTRAINED;
    // Line edits
    QSpinBox* heightSpinBox = new QSpinBox(this);
    QSpinBox* widthSpinBox = new QSpinBox(this);
    QDoubleSpinBox* interactionSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* magneticSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* temperatureSpinBox = new QDoubleSpinBox(this);
    QtLongLongSpinBox* stepsEquilSpinBox = new QtLongLongSpinBox(this);
    QtLongLongSpinBox* stepsProdSpinBox = new QtLongLongSpinBox(this);
    QSpinBox* printFreqSpinBox = new QSpinBox(this);
    
    // Buttons
    // QPushButton* applyBtn = new QPushButton("Apply",this);
    QPushButton* randomiseBtn = new QPushButton("Randomise spins", this);

    // Check Box
    // QCheckBox* constrainedBox = new QCheckBox("Constrained", this);
    // QCheckBox* printDataBox = new QCheckBox("Save data", this);
    // QCheckBox* printAverBox = new QCheckBox("Save averages", this);
};
