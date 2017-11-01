#include "parameters_widget.hpp"

#define PARAMETERS_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(interactionSpinBox); \
    Q_CHECK_PTR(magneticSpinBox);    \
    Q_CHECK_PTR(temperatureSpinBox); \
    Q_CHECK_PTR(heightSpinBox);      \
    Q_CHECK_PTR(widthSpinBox);       \
    Q_CHECK_PTR(printFreqSpinBox);   \
    Q_CHECK_PTR(stepsSpinBox);       \
    Q_CHECK_PTR(applyBtn);           \
    Q_CHECK_PTR(constrainedBox);     \
    Q_CHECK_PTR(printBox);           \


ParametersWidget::ParametersWidget(QWidget *parent)
: QWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    setMinimumWidth(300);
    // layout of this widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignVCenter);

    // add Box with Line Edits
    mainLayout->addWidget(createSystemBox());
    mainLayout->addWidget(createMCBox());
    mainLayout->addWidget(createOutputBox());
    setDefault();

    // add applyBtn
    applyBtn->setFocusPolicy(Qt::NoFocus);
    connect(applyBtn, &QPushButton::clicked, this, &ParametersWidget::applyValues);
    mainLayout->addWidget(applyBtn);

    // https://stackoverflow.com/a/16795664
    connect( interactionSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( magneticSpinBox   , static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( temperatureSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( printFreqSpinBox  , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( stepsSpinBox      , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( constrainedBox    , &QCheckBox::stateChanged, this, &ParametersWidget::valueChanged );
    connect( printBox          , &QCheckBox::stateChanged, this, &ParametersWidget::valueChanged );

    // connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), widthSpinBox, &QSpinBox::setValue );
    // connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), heightSpinBox, &QSpinBox::setValue );
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::criticalValueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::criticalValueChanged );
    

    adjustSize();
    setLayout(mainLayout);
}



ParametersWidget::~ParametersWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}




QGroupBox * ParametersWidget::createSystemBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("System Parameters");

    // default texts for LineEdits
    interactionSpinBox->setMinimum(-3);
    interactionSpinBox->setMaximum(3);
    interactionSpinBox->setDecimals(0.5);
//     interactionSpinBox->setValue(1.0);
    interactionSpinBox->setSingleStep(0.5);
    interactionSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    magneticSpinBox->setMinimum(-10);
    magneticSpinBox->setMaximum(10);
    magneticSpinBox->setDecimals(1);
//     magneticSpinBox->setValue(0.0);
    magneticSpinBox->setSingleStep(0.1);
    magneticSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    temperatureSpinBox->setMinimum(0.1);
    temperatureSpinBox->setMaximum(10);
    temperatureSpinBox->setDecimals(1);
//     temperatureSpinBox->setValue(1.0);
    temperatureSpinBox->setSingleStep(0.1);
    temperatureSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    heightSpinBox->setMinimum(1);
    heightSpinBox->setMaximum(300);
//     heightSpinBox->setValue(50);
    heightSpinBox->setSingleStep(1);
    heightSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    widthSpinBox->setMinimum(1);
    widthSpinBox->setMaximum(300);
//     widthSpinBox->setValue(50);
    widthSpinBox->setSingleStep(1);
    widthSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    // the layout
    QFormLayout* formLayout = new QFormLayout();
    // align content
    formLayout->setLabelAlignment(Qt::AlignLeft);
    // add Line Edits
    formLayout->addRow("J",interactionSpinBox);
    formLayout->addRow("H",magneticSpinBox);
    formLayout->addRow("Temperature",temperatureSpinBox);
    formLayout->addRow("System Height",heightSpinBox);
    formLayout->addRow("System Width",widthSpinBox);
    formLayout->addWidget(constrainedBox);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}



QGroupBox * ParametersWidget::createOutputBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("Output Parameters");

    // default texts for LineEdits
    printFreqSpinBox->setMinimum(0);
    printFreqSpinBox->setMaximum(100000);
    printFreqSpinBox->setSingleStep(100);
//     printFreqSpinBox->setValue(1000);
    printFreqSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // the layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->addRow("Print every",printFreqSpinBox);
    formLayout->addWidget(printBox);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}



QGroupBox * ParametersWidget::createMCBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("Monte Carlo Parameters");

    // default texts for LineEdits
    stepsSpinBox->setMinimum(0);
    stepsSpinBox->setMaximum(std::numeric_limits<qlonglong>::max());
    stepsSpinBox->setSingleStep(1000);
//     stepsSpinBox->setValue(100000000);
    stepsSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    // the layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->addRow("Simulation Steps",stepsSpinBox);
    formLayout->addWidget(constrainedBox);


    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}



void ParametersWidget::applyValues()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL
    qInfo() << "I hope this button makes you feel better :)";
}




void ParametersWidget::setReadOnly(bool flag)
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    interactionSpinBox->setReadOnly(flag);
    magneticSpinBox->setReadOnly(flag);
    temperatureSpinBox->setReadOnly(flag);
    heightSpinBox->setReadOnly(flag);
    widthSpinBox->setReadOnly(flag);
    printFreqSpinBox->setReadOnly(flag);
    stepsSpinBox->setReadOnly(flag);
    applyBtn->setEnabled(!flag);
    constrainedBox->setEnabled(!flag);
    printBox->setEnabled(!flag);
}




void ParametersWidget::setDefault()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    interactionSpinBox->setValue(1.0);
    magneticSpinBox->setValue(0.0);
    temperatureSpinBox->setValue(1.0);
    heightSpinBox->setValue(4);
    widthSpinBox->setValue(4);
    printFreqSpinBox->setValue(1);
    stepsSpinBox->setValue(10);
    constrainedBox->setEnabled(true);
    printBox->setEnabled(false);
}



double ParametersWidget::getInteraction() const
{
    Q_CHECK_PTR(interactionSpinBox);
    return interactionSpinBox->value();
}



double ParametersWidget::getMagnetic() const
{
    Q_CHECK_PTR(magneticSpinBox);
    return magneticSpinBox->value();
}



double ParametersWidget::getTemperature() const
{
    Q_CHECK_PTR(temperatureSpinBox);
    return temperatureSpinBox->value();
}



unsigned int ParametersWidget::getHeight() const
{
    Q_CHECK_PTR(heightSpinBox);
    return heightSpinBox->value();
}



unsigned int ParametersWidget::getWidth() const
{
    Q_CHECK_PTR(widthSpinBox);
    return widthSpinBox->value();
}


double ParametersWidget::getRatio() const
{
    // Q_CHECK_PTR(widthSpinBox);   // CHANGE !!!
    // return widthSpinBox->value();
    return 0.5;
}



unsigned long ParametersWidget::getSteps() const
{
    Q_CHECK_PTR(stepsSpinBox);
    return stepsSpinBox->value();
}



unsigned int ParametersWidget::getPrintFreq() const
{
    Q_CHECK_PTR(printFreqSpinBox);
    return printFreqSpinBox->value();
}


unsigned int ParametersWidget::getCorrFreq() const
{
    Q_CHECK_PTR(printFreqSpinBox);  // CHANGE !!
    return printFreqSpinBox->value();
}


unsigned int ParametersWidget::getSampleFreq() const
{
    Q_CHECK_PTR(printFreqSpinBox);  // CHANGE !!
    return printFreqSpinBox->value();
}



bool ParametersWidget::getConstrained() const
{
    Q_CHECK_PTR(constrainedBox);
    return constrainedBox->isChecked();
}



bool ParametersWidget::getPrint() const
{
    Q_CHECK_PTR(printBox);
    return printBox->isChecked();
}
