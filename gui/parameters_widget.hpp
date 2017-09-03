#pragma once


#include "global.hpp"
// #include "input/parameters.hpp"
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>



class ParametersWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParametersWidget(QWidget *parent = nullptr);
    ParametersWidget(const ParametersWidget&) = delete;
    void operator=(const ParametersWidget&) = delete;
    
    ~ParametersWidget();
    
    std::string output_key = "PLACEHOLDER";
    bool SAVETRJ = false;
    SIZE systemheigth = 0;
    SIZE systemwidth = 0;
    REAL J = 0;
    REAL H = 0;
    bool CONSTRAINED = false;
    SIZE mc_steps = 0;
    SIZE mc_printfreq = 0;
    
    
protected:
    void applyValues();
    QGroupBox* createSystemBox();
    QGroupBox* createOutputBox();
    QGroupBox* createMCBox();
    
private:
    // Line edits
    QLineEdit* interactionEdit = new QLineEdit(this);
    QLineEdit* magneticEdit = new QLineEdit(this);
    QLineEdit* heightEdit = new QLineEdit(this);
    QLineEdit* widthEdit = new QLineEdit(this);
    QLineEdit* printFreqEdit = new QLineEdit(this);
    QLineEdit* stepsEdit = new QLineEdit(this);
    
    // Buttons
    QPushButton* applyBtn = new QPushButton("Apply",this);
    QCheckBox* constrainedBox = new QCheckBox("Constrained", this);
    QCheckBox* printBox = new QCheckBox("Save trajectory", this);
};
