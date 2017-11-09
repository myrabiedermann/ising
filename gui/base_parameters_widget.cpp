#include "base_parameters_widget.hpp"

#define BASE_PARAMETERS_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(interactionSpinBox); \
    Q_CHECK_PTR(temperatureSpinBox); \
    Q_CHECK_PTR(heightSpinBox);      \
    Q_CHECK_PTR(widthSpinBox);       \
    Q_CHECK_PTR(stepsEquilSpinBox);  \
    Q_CHECK_PTR(stepsProdSpinBox);   \
    Q_CHECK_PTR(printFreqSpinBox);   \
    Q_CHECK_PTR(randomiseBtn);            



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
