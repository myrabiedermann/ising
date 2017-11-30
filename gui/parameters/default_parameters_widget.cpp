#include "default_parameters_widget.hpp"

#define DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL \
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
    Q_CHECK_PTR(stopValueSpinBox);   \
    Q_CHECK_PTR(magneticSpinBox);    



DefaultParametersWidget::DefaultParametersWidget(QWidget* parent)
 : BaseParametersWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;

    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL
    
     setMinimumWidth(300);
    
    // layout of this widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignVCenter);

    // add randomiseBtn
    randomiseBtn->setFocusPolicy(Qt::NoFocus);
    connect(randomiseBtn, &QPushButton::clicked, this, &DefaultParametersWidget::randomiseSystem);
    
    // add Box with Line Edits
    mainLayout->addWidget(createSystemBox());
    mainLayout->addWidget(randomiseBtn);
    mainLayout->addWidget(createEquilBox());
    mainLayout->addWidget(createProdBox());
    mainLayout->addWidget(createOutputBox());
    mainLayout->addWidget(createAdvancedOptionsBox());
    setDefault();
    
    
    // https://stackoverflow.com/a/16795664
    connect( interactionSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    connect( magneticSpinBox   , static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    connect( temperatureSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    connect( stepsEquilSpinBox , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    connect( stepsProdSpinBox  , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    connect( printFreqSpinBox  , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    
    // connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), widthSpinBox, &QSpinBox::setValue );
    // connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), heightSpinBox, &QSpinBox::setValue );
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DefaultParametersWidget::valueChanged );
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DefaultParametersWidget::criticalValueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DefaultParametersWidget::criticalValueChanged );
    
    adjustSize();
    setLayout(mainLayout);
}



DefaultParametersWidget::~DefaultParametersWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}



QGroupBox* DefaultParametersWidget::createSystemBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("System parameters");

    // default texts for LineEdits
    interactionSpinBox->setMinimum(-3);
    interactionSpinBox->setMaximum(3);
    interactionSpinBox->setDecimals(2);
    interactionSpinBox->setSingleStep(0.5);
    interactionSpinBox->setMinimumWidth(40);
    interactionSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    magneticSpinBox->setMinimum(-10);
    magneticSpinBox->setMaximum(10);
    magneticSpinBox->setDecimals(2);
    magneticSpinBox->setSingleStep(0.1);
    magneticSpinBox->setMinimumWidth(40);
    magneticSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    temperatureSpinBox->setMinimum(0.01);
    temperatureSpinBox->setMaximum(20);
    temperatureSpinBox->setDecimals(2);
    temperatureSpinBox->setSingleStep(0.1);
    temperatureSpinBox->setMinimumWidth(40);
    temperatureSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    heightSpinBox->setMinimum(1);
    heightSpinBox->setMaximum(300);
    heightSpinBox->setSingleStep(1);
    heightSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    widthSpinBox->setMinimum(1);
    widthSpinBox->setMaximum(300);
    widthSpinBox->setSingleStep(1);
    widthSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    // the layout
    QFormLayout* formLayout = new QFormLayout();
    // align content
    formLayout->setLabelAlignment(Qt::AlignLeft);
    // add Line Edits
    formLayout->addRow("width",widthSpinBox);
    formLayout->addRow("height",heightSpinBox);
    formLayout->addRow("interaction strength J",interactionSpinBox);
    formLayout->addRow("magnetic field B",magneticSpinBox);
    formLayout->addRow("temperature T",temperatureSpinBox);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}



QGroupBox* DefaultParametersWidget::createEquilBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

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
    formLayout->addRow("equilibration steps",stepsEquilSpinBox);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}



QGroupBox* DefaultParametersWidget::createProdBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

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
    formLayout->addRow("production steps",stepsProdSpinBox);
    formLayout->addRow("sample every ...", printFreqSpinBox); 


    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}


QGroupBox* DefaultParametersWidget::createAdvancedOptionsBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* advancedOptionsBox = new QGroupBox("Advanced Simulation Options");
    Q_CHECK_PTR(advancedOptionsBox);
    
    // set up the ComboBox:
    Q_CHECK_PTR(advancedComboBox);
    advancedComboBox->addItem("T");
    advancedComboBox->addItem("J");
    advancedComboBox->addItem("B");

    // set up the range options:
    startValueSpinBox->setDecimals(2);
    startValueSpinBox->setSingleStep(0.1);
    startValueSpinBox->setMinimum(-20);
    startValueSpinBox->setMaximum(20);

    stopValueSpinBox->setDecimals(2);
    stopValueSpinBox->setSingleStep(0.1);
    stopValueSpinBox->setMinimum(-20);
    stopValueSpinBox->setMaximum(20);

    stepValueSpinBox->setDecimals(2);
    stepValueSpinBox->setSingleStep(0.1);
    stepValueSpinBox->setMinimum(-1);
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


void DefaultParametersWidget::setReadOnly(bool flag)
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

    interactionSpinBox->setReadOnly(flag);
    temperatureSpinBox->setReadOnly(flag);
    heightSpinBox->setReadOnly(flag);
    widthSpinBox->setReadOnly(flag);
    stepsEquilSpinBox->setReadOnly(flag);
    stepsProdSpinBox->setReadOnly(flag);
    printFreqSpinBox->setReadOnly(flag);
    randomiseBtn->setEnabled(!flag);
    filenameLineEdit->setReadOnly(flag);
    advancedComboBox->setEnabled(!flag);
    startValueSpinBox->setReadOnly(flag);
    stepValueSpinBox->setReadOnly(flag);
    stopValueSpinBox->setReadOnly(flag);
    magneticSpinBox->setReadOnly(flag);
}


void DefaultParametersWidget::setDefault()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

    heightSpinBox->setValue(50);
    widthSpinBox->setValue(50);
    interactionSpinBox->setValue(1.0);
    temperatureSpinBox->setValue(1.0);
    stepsEquilSpinBox->setValue(1000000);
    stepsProdSpinBox->setValue(5000000);
    printFreqSpinBox->setValue(100);
    filenameLineEdit->setText("ising");
    advancedComboBox->setCurrentIndex(0);
    startValueSpinBox->setValue(0);
    stepValueSpinBox->setValue(0.1);
    stopValueSpinBox->setValue(0);
    magneticSpinBox->setValue(0.0);

    #ifndef NDEBUG
        heightSpinBox->setValue(6);
        widthSpinBox->setValue(6);
        stepsEquilSpinBox->setValue(5);
        stepsProdSpinBox->setValue(10);
        printFreqSpinBox->setValue(5);
    #endif
}




double DefaultParametersWidget::getMagnetic() const
{
    Q_CHECK_PTR(magneticSpinBox);
    return magneticSpinBox->value();
}

                
double DefaultParametersWidget::getRatio() const
{
    return 0.5;
}


bool DefaultParametersWidget::getConstrained() const
{
    return false;
}
     
void DefaultParametersWidget::setAdvancedValue(const double value) 
{
    Q_CHECK_PTR(advancedComboBox);
    Q_CHECK_PTR(magneticSpinBox);
    Q_CHECK_PTR(temperatureSpinBox);
    Q_CHECK_PTR(interactionSpinBox);

    if( advancedComboBox->currentIndex() == 0 )
    {
        temperatureSpinBox->setValue(value);
    }
    else if( advancedComboBox->currentIndex() == 1 )
    {
        interactionSpinBox->setValue(value);
    }
    else if( advancedComboBox->currentIndex() == 2 )
    {
        magneticSpinBox->setValue(value);
    }
    else throw std::runtime_error("something went wrong in DefaultParametersWidget::setAdvancedValue()");
}


bool DefaultParametersWidget::getWavelengthPattern() const 
{
    return false;
}
    

int DefaultParametersWidget::getWavelength() const
{
    return 0;
}


double DefaultParametersWidget::getStartValue() const 
{
    Q_CHECK_PTR(startValueSpinBox);
    return startValueSpinBox->value();
}

double DefaultParametersWidget::getStopValue() const 
{
    Q_CHECK_PTR(stopValueSpinBox);
    return stopValueSpinBox->value();
}

double DefaultParametersWidget::getStepValue() const 
{
    Q_CHECK_PTR(stepValueSpinBox);
    return stepValueSpinBox->value();
}
                
                