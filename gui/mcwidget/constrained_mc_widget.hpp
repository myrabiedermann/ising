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

    void equilibrateAction();
    void productionAction();
    void pauseAction();
    void abortAction();
    void correlateAction();

public slots:
    
signals:
    
protected:
    void serverCorrelation();
    
private:
    QPushButton* correlateBtn = new QPushButton("Compute + Save correlation", this);

};

