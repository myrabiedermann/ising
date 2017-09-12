#include "parameters_widget.hpp"



ParametersWidget::ParametersWidget(QWidget *parent) 
: QWidget(parent)
{
    setMinimumWidth(300);
    // layout of this widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignVCenter);
    
    // add Box with Line Edits
    mainLayout->addWidget(createSystemBox());
    mainLayout->addWidget(createMCBox());
    mainLayout->addWidget(createOutputBox());
    
    // add applyBtn 
    applyBtn->setFocusPolicy(Qt::NoFocus);
    connect(applyBtn, &QPushButton::clicked, this, &ParametersWidget::applyValues);
    mainLayout->addWidget(applyBtn);
    
    // https://stackoverflow.com/a/16795664
    connect( interactionSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( magneticSpinBox   , static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( printFreqSpinBox  , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( stepsSpinBox      , static_cast<void (QtLongLongSpinBox::*)(qlonglong)>(&QtLongLongSpinBox::valueChanged), this, &ParametersWidget::valueChanged );
    connect( constrainedBox    , &QCheckBox::stateChanged, this, &ParametersWidget::valueChanged );
    connect( printBox          , &QCheckBox::stateChanged, this, &ParametersWidget::valueChanged );
    
    connect( heightSpinBox     , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::criticalValueChanged );
    connect( widthSpinBox      , static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParametersWidget::criticalValueChanged );
    
    adjustSize();
    setLayout(mainLayout);
}



ParametersWidget::~ParametersWidget()
{
    // delete interactionSpinBox;
    // delete magneticSpinBox;
    // delete heightSpinBox;
    // delete widthSpinBox;
    // delete printFreqSpinBox;
    // delete stepsSpinBox;
    
    // delete applyBtn;
    // delete constrainedBox;
    // delete printBox;
}




QGroupBox * ParametersWidget::createSystemBox()
{
    // the group
    QGroupBox* labelBox = new QGroupBox("System Parameters");
    
    // default texts for LineEdits
    interactionSpinBox->setMinimum(-10);
    interactionSpinBox->setMaximum(10);
    interactionSpinBox->setDecimals(1);
    interactionSpinBox->setSingleStep(0.1);
    interactionSpinBox->setValue(1.0);
    interactionSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    magneticSpinBox->setMinimum(-10);
    magneticSpinBox->setMaximum(10);
    magneticSpinBox->setDecimals(1);
    magneticSpinBox->setSingleStep(0.1);
    magneticSpinBox->setValue(0.0);
    magneticSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    heightSpinBox->setMinimum(3);
    heightSpinBox->setMaximum(500);
    heightSpinBox->setValue(50);
    heightSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    widthSpinBox->setMinimum(3);
    widthSpinBox->setMaximum(500);
    widthSpinBox->setValue(50);
    widthSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    
    // the layout
    QFormLayout* formLayout = new QFormLayout();
    // align content
    formLayout->setLabelAlignment(Qt::AlignLeft);
    // add Line Edits
    formLayout->addRow("J",interactionSpinBox);
    formLayout->addRow("H",magneticSpinBox);
    formLayout->addRow("System Height",heightSpinBox);
    formLayout->addRow("System Width",widthSpinBox);
    formLayout->addWidget(constrainedBox);
    
    // set group layout
    labelBox->setLayout(formLayout);
    
    return labelBox;
}



QGroupBox * ParametersWidget::createOutputBox()
{
    // the group
    QGroupBox* labelBox = new QGroupBox("Output Parameters");
    
    // default texts for LineEdits
    printFreqSpinBox->setMinimum(0);
    printFreqSpinBox->setMaximum(100000);
    printFreqSpinBox->setSingleStep(100);
    printFreqSpinBox->setValue(1000);
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
    // the group
    QGroupBox* labelBox = new QGroupBox("Monte Carlo Parameters");
    
    // default texts for LineEdits
    stepsSpinBox->setMinimum(0);
    stepsSpinBox->setMaximum(std::numeric_limits<qlonglong>::max());
    stepsSpinBox->setSingleStep(1000000);
    stepsSpinBox->setValue(100000000);
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
    std::cout << "I hope this button makes you feel better :)" << std::endl;
}




void ParametersWidget::setReadOnly(bool flag)
{
    interactionSpinBox->setReadOnly(flag);
    magneticSpinBox->setReadOnly(flag);
    heightSpinBox->setReadOnly(flag);
    widthSpinBox->setReadOnly(flag);
    printFreqSpinBox->setReadOnly(flag);
    stepsSpinBox->setReadOnly(flag);
    applyBtn->setEnabled(!flag);
    constrainedBox->setEnabled(!flag);
    printBox->setEnabled(!flag);
}



float ParametersWidget::getInteraction() const
{
    return interactionSpinBox->value();
}



float ParametersWidget::getMagnetic() const
{
    return magneticSpinBox->value();
}



unsigned int ParametersWidget::getHeight() const
{
    return heightSpinBox->value();
}



unsigned int ParametersWidget::getWidth() const
{
    return widthSpinBox->value();
}



unsigned long ParametersWidget::getSteps() const
{
    return stepsSpinBox->value();
}



unsigned int ParametersWidget::getPrintFreq() const
{
    return printFreqSpinBox->value();
}



bool ParametersWidget::getConstrained() const
{
    return constrainedBox->isChecked();
}



bool ParametersWidget::getPrint() const
{
    return printBox->isChecked();
}

