#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

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
#include <QComboBox>
#include <QLabel>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QSize>
#include <limits>



class BaseParametersWidget : public QWidget
{
    Q_OBJECT
    
public:
    ~BaseParametersWidget();
    
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    double getInteraction() const;
    double getTemperature() const;
    unsigned long getStepsEquil() const;
    unsigned long getStepsProd() const;
    unsigned int  getPrintFreq() const;
    std::string getFileKey() const;

    virtual double getMagnetic() const = 0;
    virtual double getRatio() const = 0;
    
    virtual bool getConstrained() const = 0;

    double getStartValue() const;
    double getStopValue() const;
    double getStepValue() const;
    // double getAdvancedValue() const;
    virtual void setAdvancedValue(const double&) = 0;
    
public slots:
    virtual void setReadOnly(bool) = 0;
    virtual void setDefault() = 0;
    
signals:
    void valueChanged(); 
    void criticalValueChanged();
    void randomise();
    
protected:
    explicit BaseParametersWidget(QWidget* parent = Q_NULLPTR);
    BaseParametersWidget(const BaseParametersWidget&) = delete;
    void operator=(const BaseParametersWidget&) = delete;

    void randomiseSystem();
    QGroupBox* createAdvancedOptionsBox();
    QGroupBox* createOutputBox();
    virtual QGroupBox* createSystemBox() = 0;
    virtual QGroupBox* createEquilBox() = 0;
    virtual QGroupBox* createProdBox() = 0;
    virtual void setup() = 0;
    
    // Line edits
    QSpinBox* heightSpinBox = new QSpinBox(this);
    QSpinBox* widthSpinBox = new QSpinBox(this);
    QDoubleSpinBox* interactionSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* temperatureSpinBox = new QDoubleSpinBox(this);
    QtLongLongSpinBox* stepsEquilSpinBox = new QtLongLongSpinBox(this);
    QtLongLongSpinBox* stepsProdSpinBox = new QtLongLongSpinBox(this);
    QSpinBox* printFreqSpinBox = new QSpinBox(this);

    QLineEdit* filenameLineEdit = new QLineEdit(this);

    QDoubleSpinBox* magneticSpinBox {Q_NULLPTR};
    QDoubleSpinBox* ratioSpinBox {Q_NULLPTR};

    QComboBox* advancedComboBox = new QComboBox(this);
    QDoubleSpinBox* startValueSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* stopValueSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* stepValueSpinBox = new QDoubleSpinBox(this);
    
    // Buttons
    QPushButton* randomiseBtn = new QPushButton("Randomise spins", this);
};
