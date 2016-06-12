#include "workflowwidget.h"
#include <QKeyEvent>
#include <QDebug>
#include "models/order/tasksprintm.h"
#include "models/modelmanager.h"

WorkFlowWidget::WorkFlowWidget(QWidget *parent) :
    QWidget(parent), workModel(NULL), selectedWork(NULL)
{
    taskModel = new TaskModel(this);

    taskF = new SortFilterModel(this);
    taskF->setSourceModel(taskModel);
    taskF->setFilterRole(TaskModel::workIdRole);
}


void WorkFlowWidget::changeOrderStatus(Entity *order)
{
    int statusBits = 0;

    for (int row = 0; row < workModel->rowCount(); row++){
        QModelIndex indx = workModel->index(row, 0);
        Work *work = (Work *) workModel->data(indx, PPLib::entityRole).toInt();
        int status = work->fields["work_status"].toInt();
        statusBits |= (1 << status);
    }

    if ( statusBits == 1) {
        order->fields["ordr_status"] = Order::READY;

    } else if (((statusBits >> Task::PROCESS) & 1) == 1) {
        order->fields["ordr_status"] = Order::PROCESS;

    } else if (((statusBits >> Task::PAUSE) & 1) == 1) {
        order->fields["ordr_status"] = Order::PAUSE;

    } else {
        order->fields["ordr_status"] = Order::PAUSE;
    }
}


