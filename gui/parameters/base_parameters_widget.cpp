#include "base_parameters_widget.hpp"

#define BASE_PARAMETERS_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(interactionSpinBox); \
    Q_CHECK_PTR(temperatureSpinBox); \
    Q_CHECK_PTR(heightSpinBox);      \
    Q_CHECK_PTR(widthSpinBox);       \
    Q_CHECK_PTR(stepsEquilSpinBox);  \
    Q_CHECK_PTR(stepsEquilExponentSpinBox); \
    Q_CHECK_PTR(stepsProdSpinBox);   \
    Q_CHECK_PTR(stepsProdExponentSpinBox); \
    Q_CHECK_PTR(printFreqSpinBox);   \
    Q_CHECK_PTR(randomiseBtn);       \
    Q_CHECK_PTR(filenameLineEdit);    




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
    filenameLineEdit->setAlignment(Qt::AlignRight);

    // the layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignVCenter);
    formLayout->addRow("file key:",filenameLineEdit);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
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


void BaseParametersWidget::setHeight(const unsigned int H)
{
    Q_CHECK_PTR(heightSpinBox);
    return heightSpinBox->setValue(H);
}


void BaseParametersWidget::setWidth(const unsigned int W)
{
    Q_CHECK_PTR(widthSpinBox);
    return widthSpinBox->setValue(W);
}


unsigned long BaseParametersWidget::getStepsEquil() const
{
    Q_CHECK_PTR(stepsEquilSpinBox);
    return stepsEquilSpinBox->value() * std::pow(10, stepsEquilExponentSpinBox->value());
}


unsigned long BaseParametersWidget::getStepsProd() const
{
    Q_CHECK_PTR(stepsProdSpinBox);
    return stepsProdSpinBox->value() * std::pow(10, stepsProdExponentSpinBox->value());
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


