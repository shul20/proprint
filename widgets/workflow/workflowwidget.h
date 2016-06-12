#ifndef WORKFLOWWIDGET_H
#define WORKFLOWWIDGET_H

#include <QWidget>
#include "widgets/workflow/flow/flowwidget.h"
#include "models/order/taskmodel.h"
#include "models/order/workmodel.h"
#include "models/sortfiltermodel.h"
#include "entity/order/order.h"

class WorkFlowWidget : public QWidget
{
    Q_OBJECT
protected:
    QList<FlowWidget *> tasks;
    QMap<int, FlowWidget *> taskMap;
    WorkModel *workModel;
    TaskModel *taskModel;
    SortFilterModel *taskF;
    Work *selectedWork;

    void changeOrderStatus(Entity *order);

public:
    explicit WorkFlowWidget(QWidget *parent = 0);
    virtual void setOrder(Entity *order) = 0;
    virtual bool update(Entity *order, QString &message) = 0;
    virtual void setReadOnly(bool check) = 0;

signals:
    

};

#endif // WORKFLOWWIDGET_H
