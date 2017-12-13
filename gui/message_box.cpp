#include "message_box.hpp"


MessageBox::MessageBox(QWidget *parent) 
: QMessageBox(parent)
, defaultButton(Q_NULLPTR)
, constrainedButton(Q_NULLPTR)
{
    qDebug() << __PRETTY_FUNCTION__;

    setText("Program Ising");
    setInformativeText("Please choose your system type:");
    defaultButton = addButton("spin-flip", QMessageBox::YesRole);
    constrainedButton = addButton("spin-exchange", QMessageBox::YesRole);
    defaultButton->setFocusPolicy(Qt::NoFocus);
    constrainedButton->setFocusPolicy(Qt::NoFocus);
}



SYSTEMTYPE MessageBox::getType() const
{
    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(defaultButton);
    Q_CHECK_PTR(constrainedButton);

    if( clickedButton() == defaultButton )
    {
        return SYSTEMTYPE::Default;
    }
    else
    {
        return SYSTEMTYPE::Constrained;
    }
}



MessageBox::~MessageBox()
{
    qDebug() << __PRETTY_FUNCTION__;
}