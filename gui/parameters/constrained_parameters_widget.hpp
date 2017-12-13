#pragma once


#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif

#include "base_parameters_widget.hpp"
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



class ConstrainedParametersWidget : public BaseParametersWidget
{
    Q_OBJECT
    
public:
    explicit ConstrainedParametersWidget(QWidget *parent = Q_NULLPTR);
    ConstrainedParametersWidget(const ConstrainedParametersWidget&) = delete;
    void operator=(const ConstrainedParametersWidget&) = delete;
    
    ~ConstrainedParametersWidget();

    double getMagnetic() const;
    double getRatio() const;
    bool   getWavelengthPattern() const;
    int    getWavelength() const;
    bool   getConstrained() const;
    double getStartValue() const;
    double getStopValue() const;
    double getStepValue() const;
    bool   getAdvancedRandomise() const;

    void setAdvancedValue(const double);
    
public slots:
    void setReadOnly(bool);
    void setDefault();
    void setRatioWavelengthCheckBox();
    
signals:
    
protected:
    QGroupBox* createSystemBox();
    QGroupBox* createEquilBox();
    QGroupBox* createProdBox();
    QGroupBox* createAdvancedOptionsBox();
    
private:
    QDoubleSpinBox* ratioSpinBox = new QDoubleSpinBox(this);
    QCheckBox*      ratioCheckBox = new QCheckBox(this);
    QSpinBox*       wavelengthSpinBox = new QSpinBox(this);
    QCheckBox*      wavelengthCheckBox = new QCheckBox(this);

};
