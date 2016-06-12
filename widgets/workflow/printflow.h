#ifndef PRINTFLOW_H
#define PRINTFLOW_H

#include <QWidget>
#include "widgets/workflow/workflowwidget.h"
#include "widgets/workflow/flow/flowwidget.h"
namespace Ui {
class PrintFlow;
}

class PrintFlow : public WorkFlowWidget
{
    Q_OBJECT
    
public:
    explicit PrintFlow(QWidget *parent = 0);
    ~PrintFlow();

    void setOrder(Entity *order);
    bool update(Entity *order, QString &message);
    void setReadOnly(bool set);
    
private:
    Ui::PrintFlow *ui;
    bool isReadOnly;
    Entity *order;
    QWidget *scrollContent;

private slots:
    void workSelected(QModelIndex index);
};

#endif // PRINTFLOW_H
