#include "base_parameters_widget.hpp"

#define BASE_PARAMETERS_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(interactionSpinBox); \
    Q_CHECK_PTR(temperatureSpinBox); \
    Q_CHECK_PTR(heightSpinBox);      \
    Q_CHECK_PTR(widthSpinBox);       \
    Q_CHECK_PTR(stepsEquilSpinBox);  \
    Q_CHECK_PTR(stepsProdSpinBox);   \
    Q_CHECK_PTR(printFreqSpinBox);   \
    Q_CHECK_PTR(randomiseBtn);       \
    Q_CHECK_PTR(filenameLineEdit);   \
    Q_CHECK_PTR(advancedComboBox);   \
    Q_CHECK_PTR(startValueSpinBox);  \
    Q_CHECK_PTR(stepValueSpinBox);   \
    Q_CHECK_PTR(stopValueSpinBox);   




BaseParametersWidget::BaseParametersWidget(QWidget *parent)
: QWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    BASE_PARAMETERS_WIDGET_ASSERT_ALL
}

BaseParametersWidget::~BaseParametersWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}


void BaseParametersWidget::randomiseSystem()
{
    qDebug() << __PRETTY_FUNCTION__;
    BASE_PARAMETERS_WIDGET_ASSERT_ALL
    qInfo() << "Randomise button was hit!";
    emit randomise();
}



QGroupBox* BaseParametersWidget::createOutputBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    BASE_PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("Output parameters");
    
    // default texts for LineEdits
    filenameLineEdit->setMaxLength(20);
    filenameLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // the layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignVCenter);
    formLayout->addRow("file key:",filenameLineEdit);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}


QGroupBox* BaseParametersWidget::createAdvancedOptionsBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    BASE_PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* advancedOptionsBox = new QGroupBox("Advanced Simulation Options");
    Q_CHECK_PTR(advancedOptionsBox);
    
    // set up the ComboBox:
    Q_CHECK_PTR(advancedComboBox);
    advancedComboBox->addItem("T");
    advancedComboBox->addItem("B");
    advancedComboBox->addItem("J");

    // set up the range options:
    startValueSpinBox->setDecimals(1);
    startValueSpinBox->setSingleStep(0.1);
    startValueSpinBox->setMinimum(-10);
    startValueSpinBox->setMaximum(10);

    stopValueSpinBox->setDecimals(1);
    stopValueSpinBox->setSingleStep(0.1);
    stopValueSpinBox->setMinimum(-10);
    stopValueSpinBox->setMaximum(10);

    stepValueSpinBox->setDecimals(1);
    stepValueSpinBox->setSingleStep(0.1);
    stepValueSpinBox->setMinimum(0.1);
    stepValueSpinBox->setMaximum(1);

    startValueSpinBox->setMinimumWidth(50);
    startValueSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    stopValueSpinBox->setMinimumWidth(50);
    stopValueSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    stepValueSpinBox->setMinimumWidth(50);
    stepValueSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // the layout 
    QFormLayout* formLayout = new QFormLayout();
    Q_CHECK_PTR(formLayout);
    formLayout->setLabelAlignment(Qt::AlignCenter);

    QLabel *label = new QLabel(this);
    label->setFrameStyle(QFrame::NoFrame);
    label->setText("produce a range of MC simulations");
    label->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    formLayout->addRow(label);
    formLayout->addRow("parameter to vary", advancedComboBox);
    
    QHBoxLayout* rangeOptions = new QHBoxLayout();
    Q_CHECK_PTR(rangeOptions);
    rangeOptions->addWidget(startValueSpinBox);
    rangeOptions->addWidget(stepValueSpinBox);
    rangeOptions->addWidget(stopValueSpinBox);
    formLayout->addRow("start : step : end", rangeOptions);

    advancedOptionsBox->setLayout(formLayout);
    return advancedOptionsBox;
}



double BaseParametersWidget::getInteraction() const
{
    Q_CHECK_PTR(interactionSpinBox);
    return interactionSpinBox->value();
}


double BaseParametersWidget::getTemperature() const
{
    Q_CHECK_PTR(temperatureSpinBox);
    return temperatureSpinBox->value();
}


unsigned int BaseParametersWidget::getHeight() const
{
    Q_CHECK_PTR(heightSpinBox);
    return heightSpinBox->value();
}


unsigned int BaseParametersWidget::getWidth() const
{
    Q_CHECK_PTR(widthSpinBox);
    return widthSpinBox->value();
}


unsigned long BaseParametersWidget::getStepsEquil() const
{
    Q_CHECK_PTR(stepsEquilSpinBox);
    return stepsEquilSpinBox->value();
}


unsigned long BaseParametersWidget::getStepsProd() const
{
    Q_CHECK_PTR(stepsProdSpinBox);
    return stepsProdSpinBox->value();
}


unsigned int BaseParametersWidget::getPrintFreq() const
{
    Q_CHECK_PTR(printFreqSpinBox);
    return printFreqSpinBox->value();
}

std::string BaseParametersWidget::getFileKey() const
{

    Q_CHECK_PTR(filenameLineEdit);
    return filenameLineEdit->displayText().toStdString();
}

double BaseParametersWidget::getStartValue() const 
{
    Q_CHECK_PTR(startValueSpinBox);
    return startValueSpinBox->value();
}

double BaseParametersWidget::getStopValue() const 
{
    Q_CHECK_PTR(stopValueSpinBox);
    return stopValueSpinBox->value();
}

double BaseParametersWidget::getStepValue() const 
{
    Q_CHECK_PTR(stepValueSpinBox);
    return stepValueSpinBox->value();
}

// double BaseParametersWidget::getAdvancedValue() const 
// {
//     Q_CHECK_PTR(advancedComboBox);
//     return advancedComboBox->currentIndex();
// }