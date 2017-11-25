#pragma once

#ifdef QT_NO_DEBUG
    #ifndef QT_NO_DEBUG_OUTPUT
        #define QT_NO_DEBUG_OUTPUT
    #endif
#endif


#include "mcwidget/base_mc_widget.hpp"



class DefaultMCWidget : public BaseMCWidget
{
    Q_OBJECT
    
public:
    explicit DefaultMCWidget(QWidget* parent = Q_NULLPTR);
    DefaultMCWidget(const DefaultMCWidget&) = delete;
    void operator=(const DefaultMCWidget&) = delete;
    
    ~DefaultMCWidget();
    
    void advancedRunAction() override;

public slots:
    
signals:
    
protected:
    
private:
    void setup();
    QPushButton* advancedRunBtn = new QPushButton("Advanced Simulation Scheme", this);

};

