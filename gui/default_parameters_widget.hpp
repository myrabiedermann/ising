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



class DefaultParametersWidget : public BaseParametersWidget
{
    Q_OBJECT
    
public:
    explicit DefaultParametersWidget(QWidget *parent = Q_NULLPTR);
    DefaultParametersWidget(const DefaultParametersWidget&) = delete;
    void operator=(const DefaultParametersWidget&) = delete;
    
    ~DefaultParametersWidget();

    double getMagnetic() const;
    double getRatio() const;

    bool getConstrained() const;
    void setAdvancedValue(const double&);
    
public slots:
    void setReadOnly(bool);
    void setDefault();
    
signals:
    
protected:
    QGroupBox* createSystemBox();
    QGroupBox* createEquilBox();
    QGroupBox* createProdBox();
    void setup();
    
private:

};
