#pragma once


#include "global.hpp"
// #include "input/parameters.hpp"
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
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
    
protected:
    void applyValues();
    QGroupBox* createSystemBox();
    QGroupBox* createOutputBox();
    QGroupBox* createMCBox();
    
private:
    // Line edits
    QDoubleSpinBox* interactionSpinBox = new QDoubleSpinBox(this);
    QDoubleSpinBox* magneticSpinBox = new QDoubleSpinBox(this);
    QSpinBox* heightSpinBox = new QSpinBox(this);
    QSpinBox* widthSpinBox = new QSpinBox(this);
    QSpinBox* printFreqSpinBox = new QSpinBox(this);
    QSpinBox* stepsSpinBox = new QSpinBox(this);
    
    // Buttons
    QPushButton* applyBtn = new QPushButton("Apply",this);
    QCheckBox* constrainedBox = new QCheckBox("Constrained", this);
    QCheckBox* printBox = new QCheckBox("Save trajectory", this);
    
//     // Actual parameters
//     std::string output_key = "PLACEHOLDER";
//     bool SAVETRJ = false;
//     SIZE systemheigth = 0;
//     SIZE systemwidth = 0;
//     REAL J = 0;
//     REAL H = 0;
//     bool CONSTRAINED = false;
//     SIZE mc_steps = 0;
//     SIZE mc_printfreq = 0;
};
