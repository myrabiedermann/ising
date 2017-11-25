#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "mcwidget/base_mc_widget.hpp"



class ConstrainedMCWidget : public BaseMCWidget
{
    Q_OBJECT
    
public:
    explicit ConstrainedMCWidget(QWidget* parent = Q_NULLPTR);
    ConstrainedMCWidget(const ConstrainedMCWidget&) = delete;
    void operator=(const ConstrainedMCWidget&) = delete;
    
    ~ConstrainedMCWidget();

public slots:
    
signals:
    
protected:
    
private:
    void setup();

};

