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
    Q_CHECK_PTR(magneticSpinBox);    



DefaultParametersWidget::DefaultParametersWidget(QWidget* parent)
 : BaseParametersWidget(parent)
{
    qDebug() << __PRETTY_FUNCTION__;

    magneticSpinBox = new QDoubleSpinBox(this);
    // ratioSpinBox = new QDoubleSpinBox(this);

    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL
    setup();

}



DefaultParametersWidget::~DefaultParametersWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}


void DefaultParametersWidget::setup()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

    setMinimumWidth(300);
    
    // layout of this widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignVCenter);

    
    // add Box with Line Edits
    mainLayout->addWidget(createSystemBox());
    mainLayout->addWidget(createEquilBox());
    mainLayout->addWidget(createProdBox());
    mainLayout->addWidget(createOutputBox());
    setDefault();
    
    // add randomiseBtn
    randomiseBtn->setFocusPolicy(Qt::NoFocus);
    connect(randomiseBtn, &QPushButton::clicked, this, &DefaultParametersWidget::randomiseSystem);
    mainLayout->addWidget(randomiseBtn);
    
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




QGroupBox* DefaultParametersWidget::createSystemBox()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

    // the group
    QGroupBox* labelBox = new QGroupBox("System parameters");

    // default texts for LineEdits
    interactionSpinBox->setMinimum(-3);
    interactionSpinBox->setMaximum(3);
    interactionSpinBox->setDecimals(1);
    interactionSpinBox->setSingleStep(0.5);
    interactionSpinBox->setMinimumWidth(40);
    interactionSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    magneticSpinBox->setMinimum(-10);
    magneticSpinBox->setMaximum(10);
    magneticSpinBox->setDecimals(1);
    magneticSpinBox->setSingleStep(0.1);
    magneticSpinBox->setMinimumWidth(40);
    magneticSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    temperatureSpinBox->setMinimum(0.1);
    temperatureSpinBox->setMaximum(10);
    temperatureSpinBox->setDecimals(1);
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
    formLayout->addRow("Equilibration steps",stepsEquilSpinBox);

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
    formLayout->addRow("Production steps",stepsProdSpinBox);
    formLayout->addRow("sample every ...", printFreqSpinBox); 


    // set group layout
    labelBox->setLayout(formLayout);

    return labelBox;
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
    magneticSpinBox->setReadOnly(flag);
}




void DefaultParametersWidget::setDefault()
{
    qDebug() << __PRETTY_FUNCTION__;
    DEFAULT_PARAMETERS_WIDGET_ASSERT_ALL

    heightSpinBox->setValue(4);
    widthSpinBox->setValue(4);
    interactionSpinBox->setValue(1.0);
    temperatureSpinBox->setValue(2.0);
    stepsEquilSpinBox->setValue(10000);
    stepsProdSpinBox->setValue(10000);
    printFreqSpinBox->setValue(10);
    filenameLineEdit->setText("ising");
    magneticSpinBox->setValue(1.0);
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
                
                
                