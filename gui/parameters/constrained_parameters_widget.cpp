#include "constrained_parameters_widget.hpp"

#define CONSTRAINED_PARAMETERS_WIDGET_ASSERT_ALL \
    Q_CHECK_PTR(interactionSpinBox); \
    Q_CHECK_PTR(temperatureSpinBox); \
    Q_CHECK_PTR(heightSpinBox);      \
    Q_CHECK_PTR(widthSpinBox);       \
    Q_CHECK_PTR(stepsEquilSpinBox);  \
    Q_CHECK_PTR(stepsProdSpinBox);   \
    Q_CHECK_PTR(printFreqSpinBox);   \
    Q_CHECK_PTR(randomiseBtn);       \
    Q_CHECK_PTR(filenameLineEdit);   \
    Q_CHECK_PTR(wavelengthSpinBox);  \
    Q_CHECK_PTR(wavelengthCheckBox); \
    Q_CHECK_PTR(ratioCheckBox); \
    Q_CHECK_PTR(ratioSpinBox);    



ConstrainedParametersWidget::ConstrainedParametersWidget(QWidget* parent)
 : BaseParametersWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;

    CONSTRAINED_PARAMETERS_WIDGET_ASSERT_ALL

    wavelengthCheckBox->setCheckable(true);
    ratioCheckBox->setCheckable(true);
    wavelengthCheckBox->setAutoExclusive(true);
    ratioCheckBox->setAutoExclusive(true);
    ratioCheckBox->setChecked(true);

    setMinimumWidth(300);
    
    // layout of this widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignVCenter);

    // add randomiseBtn
    randomiseBtn->setFocusPolicy(Qt::NoFocus);
    connect(randomiseBtn, &QPushButton::clicked, this, &ConstrainedParametersWidget::randomiseSystem);
    
    // add Box with Line Edits
    mainLayout->addWidget(createSystemBox());
    mainLayout->addWidget(randomiseBtn);
    mainLayout->addWidget(createEquilBox());
    mainLayout->addWidget(createProdBox());
    mainLayout->addWidget(createOutputBox());
    setDefault();
    
    // https://stackoverflow.com/a/16795664
    connect( interactionSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ConstrainedParametersWidget::valueChanged );
    connect( temperatureSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ConstrainedParametersWidget::valueChanged );
    connect( stepsEquilSpinBox , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &ConstrainedParametersWidget::valueChanged );
    connect( stepsProdSpinBox  , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &ConstrainedParametersWidget::valueChanged );
    connect( printFreqSpinBox  , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ConstrainedParametersWidget::valueChanged );
    
    connect( ratioSpinBox      , static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ConstrainedParametersWidget::randomiseSystem );
    connect( ratioCheckBox     , static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged), this, &ConstrainedParametersWidget::randomiseSystem );
    connect( wavelengthSpinBox , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ConstrainedParametersWidget::randomiseSystem);
    connect( wavelengthCheckBox, static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged), this, &ConstrainedParametersWidget::randomiseSystem );

    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), widthSpinBox, &QSpinBox::setValue );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), heightSpinBox, &QSpinBox::setValue );
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ConstrainedParametersWidget::valueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ConstrainedParametersWidget::valueChanged );
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ConstrainedParametersWidget::criticalValueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ConstrainedParametersWidget::criticalValueChanged );
    
    adjustSize();
    setLayout(mainLayout);

}



ConstrainedParametersWidget::~ConstrainedParametersWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}




QGroupBox* ConstrainedParametersWidget::createSystemBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("System parameters");

    // default texts for LineEdits
    interactionSpinBox->setMinimum(-3);
    interactionSpinBox->setMaximum(3);
    interactionSpinBox->setDecimals(2);
    interactionSpinBox->setSingleStep(0.5);
    interactionSpinBox->setMinimumWidth(40);
    interactionSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    ratioSpinBox->setMinimum(0.1);
    ratioSpinBox->setMaximum(0.9);
    ratioSpinBox->setSingleStep(0.1);
    ratioSpinBox->setDecimals(1);
    ratioSpinBox->setMinimumWidth(40);
    ratioSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    temperatureSpinBox->setMinimum(0.1);
    temperatureSpinBox->setMaximum(10);
    temperatureSpinBox->setDecimals(2);
    temperatureSpinBox->setSingleStep(0.1);
    temperatureSpinBox->setMinimumWidth(40);
    temperatureSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    heightSpinBox->setMinimum(2);
    heightSpinBox->setMaximum(500);
    heightSpinBox->setSingleStep(2);
    heightSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    widthSpinBox->setMinimum(2);
    widthSpinBox->setMaximum(500);
    widthSpinBox->setSingleStep(2);
    widthSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    wavelengthSpinBox->setMinimum(1);
    wavelengthSpinBox->setMaximum(20);
    wavelengthSpinBox->setSingleStep(1);
    wavelengthSpinBox->setMinimumWidth(40);
    wavelengthSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    

    // the layout
    QFormLayout* formLayout = new QFormLayout();
    // align content
    formLayout->setLabelAlignment(Qt::AlignLeft);
    // add Line Edits
    formLayout->addRow("width",widthSpinBox);
    formLayout->addRow("height",heightSpinBox);
    formLayout->addRow("interaction strength J",interactionSpinBox);
    formLayout->addRow("temperature T",temperatureSpinBox);
    formLayout->addRow("initial random pattern", ratioCheckBox);
    formLayout->addRow("ratio B to A [0.1:0.9]",ratioSpinBox);
    formLayout->addRow("initial cos(ky) pattern", wavelengthCheckBox);
    formLayout->addRow("wavelength k", wavelengthSpinBox);

    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
}


QGroupBox* ConstrainedParametersWidget::createEquilBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_PARAMETERS_WIDGET_ASSERT_ALL

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



QGroupBox* ConstrainedParametersWidget::createProdBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_PARAMETERS_WIDGET_ASSERT_ALL

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


QGroupBox* ConstrainedParametersWidget::createAdvancedOptionsBox()
{
    return Q_NULLPTR;
}


void ConstrainedParametersWidget::setReadOnly(bool flag)
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_PARAMETERS_WIDGET_ASSERT_ALL

    interactionSpinBox->setReadOnly(flag);
    temperatureSpinBox->setReadOnly(flag);
    heightSpinBox->setReadOnly(flag);
    widthSpinBox->setReadOnly(flag);
    stepsEquilSpinBox->setReadOnly(flag);
    stepsProdSpinBox->setReadOnly(flag);
    printFreqSpinBox->setReadOnly(flag);
    randomiseBtn->setEnabled(!flag);
    filenameLineEdit->setReadOnly(flag);
    ratioSpinBox->setReadOnly(flag);
    ratioCheckBox->setEnabled(!flag);
    wavelengthSpinBox->setReadOnly(flag);
    wavelengthCheckBox->setEnabled(!flag);

}




void ConstrainedParametersWidget::setDefault()
{
    qDebug() << __PRETTY_FUNCTION__;
    CONSTRAINED_PARAMETERS_WIDGET_ASSERT_ALL

    heightSpinBox->setValue(60);
    widthSpinBox->setValue(60);
    interactionSpinBox->setValue(1.0);
    temperatureSpinBox->setValue(1.0);
    stepsEquilSpinBox->setValue(1000000);
    stepsProdSpinBox->setValue(5000000);
    printFreqSpinBox->setValue(100);
    filenameLineEdit->setText("ising");
    ratioSpinBox->setValue(0.5);
    ratioCheckBox->click();
    wavelengthSpinBox->setValue(1);
    
    #ifndef NDEBUG
        heightSpinBox->setValue(6);
        widthSpinBox->setValue(6);
        stepsEquilSpinBox->setValue(5);
        stepsProdSpinBox->setValue(10);
        printFreqSpinBox->setValue(5);
    #endif
}




double ConstrainedParametersWidget::getMagnetic() const
{
    return 0;
}

                
double ConstrainedParametersWidget::getRatio() const
{
    Q_CHECK_PTR(ratioSpinBox);
    return ratioSpinBox->value();
}


bool ConstrainedParametersWidget::getConstrained() const
{
    return true;
}

void ConstrainedParametersWidget::setAdvancedValue( __attribute__((unused)) const double value)
{}
                

bool ConstrainedParametersWidget::getWavelengthPattern() const 
{
    Q_CHECK_PTR(wavelengthCheckBox);
    return wavelengthCheckBox->isChecked();
}
    

int ConstrainedParametersWidget::getWavelength() const
{
    Q_CHECK_PTR(wavelengthSpinBox);
    return wavelengthSpinBox->value();
}

double ConstrainedParametersWidget::getStartValue() const 
{
    return 0;
}

double ConstrainedParametersWidget::getStopValue() const 
{
    return 0;
}

double ConstrainedParametersWidget::getStepValue() const 
{
    return 0;
}

bool ConstrainedParametersWidget::getAdvancedRandomise() const
{
    return false;
}
         