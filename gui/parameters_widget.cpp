#include "parameters_widget.hpp"



ParametersWidget::ParametersWidget(QWidget *parent) 
: QWidget(parent)
{
    // layout of this widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // add Box with Line Edits
    mainLayout->addWidget(createSystemBox());
    mainLayout->addWidget(createOutputBox());
    mainLayout->addWidget(createMCBox());
    
    // add applyBtn 
    connect(applyBtn, &QPushButton::clicked, this, &ParametersWidget::applyValues);
    mainLayout->addWidget(applyBtn);
    mainLayout->setAlignment(Qt::AlignVCenter);
    
    setMinimumWidth(300);
    adjustSize();
    setLayout(mainLayout);
}



ParametersWidget::~ParametersWidget()
{
    delete interactionEdit;
    delete magneticEdit;
    delete heightEdit;
    delete widthEdit;
}



QGroupBox * ParametersWidget::createSystemBox()
{
    // the group
    QGroupBox* labelBox = new QGroupBox("System Parameters");
    
    // default texts for LineEdits
    interactionEdit->setText("1.0");
    magneticEdit->setText("0.0");
    heightEdit->setText("50");
    widthEdit->setText("50");
    
    // the layout
    QFormLayout* formLayout = new QFormLayout();
    // align content
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignRight);
    // add Line Edits
    formLayout->addRow("J",interactionEdit);
    formLayout->addRow("H",magneticEdit);
    formLayout->addRow("System Height",heightEdit);
    formLayout->addRow("System Width",widthEdit);
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
    printFreqEdit->setText("1000");
    
    // the layout
    QFormLayout* formLayout = new QFormLayout();
    // align content
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignRight);
    // add Line Edits
    formLayout->addRow("Print every",printFreqEdit);
    // add checkbox
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
    stepsEdit->setText("1000000");
    
    // the layout
    QFormLayout* formLayout = new QFormLayout();
    // align content
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignRight);
    // add Line Edits
    formLayout->addRow("Simulation Steps",stepsEdit);
    
    // set group layout
    labelBox->setLayout(formLayout);
    
    return labelBox;
}



void ParametersWidget::applyValues()
{
    
}
