#include "message_box.hpp"


MessageBox::MessageBox(QWidget *parent) :
QMessageBox(parent),
defaultButton(Q_NULLPTR),
constrainedButton(Q_NULLPTR)
{
    qDebug() << __PRETTY_FUNCTION__;

    setText("Program Ising");
    setInformativeText("Please choose your system type:");
    setDetailedText("Default: \nstandard Ising system \n\nConstrained: \nIsing system constrained to # up-spins = # down-spins");
    defaultButton = addButton("Default", QMessageBox::YesRole);
    constrainedButton = addButton("Constrained", QMessageBox::NoRole);

}



SYSTEMTYPE MessageBox::getType() const
{
    Q_CHECK_PTR(defaultButton);
    Q_CHECK_PTR(constrainedButton);

    if( clickedButton() == defaultButton )
    {
        return SYSTEMTYPE::Default;
    }
    else if( clickedButton() == constrainedButton )
    {
        return SYSTEMTYPE::Constrained;
    }
    else
    {
        return SYSTEMTYPE::Default;
    }
}



MessageBox::~MessageBox()
{
    qDebug() << __PRETTY_FUNCTION__;
}