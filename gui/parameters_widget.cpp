#include "parameters_widget.hpp"

#define PARAMETERS_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(interactionSpinBox); \
    Q_CHECK_PTR(magneticSpinBox);    \
    Q_CHECK_PTR(temperatureSpinBox); \
    Q_CHECK_PTR(heightSpinBox);      \
    Q_CHECK_PTR(widthSpinBox);       \
    Q_CHECK_PTR(stepsEquilSpinBox);  \
    Q_CHECK_PTR(stepsProdSpinBox);   \
    Q_CHECK_PTR(printFreqSpinBox);   \
    Q_CHECK_PTR(randomiseBtn);       \


ParametersWidget::ParametersWidget(QWidget *parent)
: QWidget(parent),
CONSTRAINED(false)
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    setMinimumWidth(300);
    // layout of this widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignVCenter);

    // add Box with Line Edits
    mainLayout->addWidget(createSystemBox());
    mainLayout->addWidget(createEquilBox());
    mainLayout->addWidget(createProdBox());
    setDefault();

    // add applyBtn
    // applyBtn->setFocusPolicy(Qt::NoFocus);
    // connect(applyBtn, &QPushButton::clicked, this, &ParametersWidget::applyValues);
    // mainLayout->addWidget(applyBtn);
    
    // add randomiseBtn
    randomiseBtn->setFocusPolicy(Qt::NoFocus);
    connect(randomiseBtn, &QPushButton::clicked, this, &ParametersWidget::randomiseSystem);
    mainLayout->addWidget(randomiseBtn);

    // https://stackoverflow.com/a/16795664
    connect( interactionSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( magneticSpinBox   , static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( temperatureSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( stepsEquilSpinBox , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( stepsProdSpinBox  , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( printFreqSpinBox  , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    // connect( constrainedBox    , &QCheckBox::stateChanged, this, &ParametersWidget::valueChanged );
    // connect( printDataBox      , &QCheckBox::stateChanged, this, &ParametersWidget::valueChanged );
    // connect( printAverBox      , &QCheckBox::stateChanged, this, &ParametersWidget::valueChanged );

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
    QGroupBox* labelBox = new QGroupBox("System parameters");

    // default texts for LineEdits
    interactionSpinBox->setMinimum(-3);
    interactionSpinBox->setMaximum(3);
    interactionSpinBox->setDecimals(1);
//     interactionSpinBox->setValue(1.0);
    interactionSpinBox->setSingleStep(0.5);
    interactionSpinBox->setMinimumWidth(40);
    interactionSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    magneticSpinBox->setMinimum(-10);
    magneticSpinBox->setMaximum(10);
    magneticSpinBox->setDecimals(1);
//     magneticSpinBox->setValue(0.0);
    magneticSpinBox->setSingleStep(0.1);
    magneticSpinBox->setMinimumWidth(40);
    magneticSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    temperatureSpinBox->setMinimum(0.1);
    temperatureSpinBox->setMaximum(10);
    temperatureSpinBox->setDecimals(1);
//     temperatureSpinBox->setValue(1.0);
    temperatureSpinBox->setSingleStep(0.1);
    temperatureSpinBox->setMinimumWidth(40);
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
    formLayout->addRow("width",widthSpinBox);
    formLayout->addRow("height",heightSpinBox);
    formLayout->addRow("J",interactionSpinBox);
    formLayout->addRow("B",magneticSpinBox);
    formLayout->addRow("Temperature",temperatureSpinBox);
    // formLayout->addWidget(constrainedBox);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}



QGroupBox* ParametersWidget::createEquilBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL
    
    // the group
    QGroupBox* labelBox = new QGroupBox("Equilibration parameters");
    
    // default texts for LineEdits
    stepsEquilSpinBox->setMinimum(0);
    stepsEquilSpinBox->setMaximum(std::numeric_limits<qlonglong>::max());
    stepsEquilSpinBox->setSingleStep(1000);
    stepsEquilSpinBox->setMinimumWidth(100);
    stepsEquilSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // the layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignVCenter);
    formLayout->addRow("Equilibration steps",stepsEquilSpinBox);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}



QGroupBox* ParametersWidget::createProdBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("Production parameters");

    // default texts for LineEdits
    printFreqSpinBox->setMinimum(0);
    printFreqSpinBox->setMaximum(1000000);
    printFreqSpinBox->setSingleStep(10);
    printFreqSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    stepsProdSpinBox->setMinimum(0);
    stepsProdSpinBox->setMaximum(std::numeric_limits<qlonglong>::max());
    stepsProdSpinBox->setSingleStep(1000);
    stepsProdSpinBox->setMinimumWidth(100);
    stepsProdSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // the layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignHCenter);
    formLayout->addRow("Production steps",stepsProdSpinBox);
    // formLayout->addWidget(printDataBox);
    // formLayout->addWidget(printAverBox);
    formLayout->addRow("save every ...", printFreqSpinBox); 


    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}




// void ParametersWidget::applyValues()
// {
//     qDebug() << __PRETTY_FUNCTION__;
//     PARAMETERS_WIDGET_ASSERT_ALL
//     qInfo() << "Apply button was hit!";
//     emit valueChanged();
// }



void ParametersWidget::randomiseSystem()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL
    qInfo() << "Randomise button was hit!";
    emit randomise();
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
    stepsEquilSpinBox->setReadOnly(flag);
    stepsProdSpinBox->setReadOnly(flag);
    printFreqSpinBox->setReadOnly(flag);
    // applyBtn->setEnabled(!flag);
    randomiseBtn->setEnabled(!flag);
    // constrainedBox->setEnabled(!flag);
    // printDataBox->setEnabled(!flag);
    // printAverBox->setEnabled(!flag);
}




void ParametersWidget::setDefault()
{
    qDebug() << __PRETTY_FUNCTION__;
    PARAMETERS_WIDGET_ASSERT_ALL

    heightSpinBox->setValue(10);
    widthSpinBox->setValue(10);
    interactionSpinBox->setValue(1.0);
    magneticSpinBox->setValue(0.0);
    temperatureSpinBox->setValue(2.0);
    // constrainedBox->setEnabled(true);
    stepsEquilSpinBox->setValue(1000);
    stepsProdSpinBox->setValue(1000);
    printFreqSpinBox->setValue(10);
    // printDataBox->setEnabled(true);
    // printAverBox->setEnabled(true);
}



double ParametersWidget::getInteraction() const
{
    Q_CHECK_PTR(interactionSpinBox);
    return interactionSpinBox->value();
}



double ParametersWidget::getMagnetic() const
{
    Q_CHECK_PTR(magneticSpinBox);
    if( CONSTRAINED )
        return 0.0;
    else
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



unsigned long ParametersWidget::getStepsEquil() const
{
    Q_CHECK_PTR(stepsEquilSpinBox);
    return stepsEquilSpinBox->value();
}



unsigned long ParametersWidget::getStepsProd() const
{
    Q_CHECK_PTR(stepsProdSpinBox);
    return stepsProdSpinBox->value();
}



unsigned int ParametersWidget::getPrintFreq() const
{
    Q_CHECK_PTR(printFreqSpinBox);
    return printFreqSpinBox->value();
}



void ParametersWidget::setConstrained(const bool constrained) 
{
    CONSTRAINED = constrained;
} 



bool ParametersWidget::getConstrained() const
{
    return CONSTRAINED;
}



// bool ParametersWidget::getPrintData() const
// {
//     Q_CHECK_PTR(printDataBox);
//     return printDataBox->isChecked();
// }



// bool ParametersWidget::getPrintAver() const
// {
//     Q_CHECK_PTR(printAverBox);
//     return printAverBox->isChecked();
// }


