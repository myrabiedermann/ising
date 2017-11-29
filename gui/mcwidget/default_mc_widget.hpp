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
    
    void advancedRunAction();
    void equilibrateAction();
    void productionAction();
    void pauseAction();
    void abortAction();

public slots:
    
signals:
    
private:
    QPushButton* advancedRunBtn = new QPushButton("Advanced Simulation Scheme", this);
    std::vector<double> advancedValues {};
    std::atomic<unsigned long> advancedEquilMode {true};
    std::atomic<unsigned long> advancedCycleDone {true};

    void serverAdvanced();

};

