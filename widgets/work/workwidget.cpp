#include "workwidget.h"
#include "utility/process.h"

WorkWidget::WorkWidget(QWidget *parent) :
    QWidget(parent), workModel(NULL), idCounter(-1)
{
    taskModel = new TaskModel(this);
    taskF = new SortFilterModel(this);
    taskF->setSourceModel(taskModel);
    taskF->setFilterRole(TaskModel::workIdRole);
}

void WorkWidget::setWidgets(PriceTypeWidget *priceType, CompanyWidget *company)
{
    priceTypeW = priceType;
    companyW = company;

    foreach (TaskWidget *taskW, taskMap) {
        connect(taskW, SIGNAL(setCheck(bool)),
                this, SLOT(registerTask(bool)));
    }
}

Size &WorkWidget::getTotalSize()
{
    return totalSize;
}

void WorkWidget::setTotalSize(Size value)
{
    totalSize = value;
    emit worksChaged();
}

Money &WorkWidget::getTotal()
{
    return total;
}

void WorkWidget::setTotal(Money total, Money print)
{
    this->total = total;
    totalPrint = print;
    emit worksChaged();
}

Money &WorkWidget::getTotalPrint()
{
    return totalPrint;
}

bool WorkWidget::addTasks2Work(Entity *work)
{
    int tasksBits = 0;
    foreach (TaskWidget *taskW, tasks) {
        Task *task = new Task();
        task->fields["work_id"] = work->getId();
        if (!taskW->form2Entity(task) ||
            !taskModel->setData(QModelIndex(), (int) task, PPLib::addRole)){
            delete task;
            return false;
        }
        tasksBits |= 1 << task->fields["task_process"].toInt();
    }
    work->fields["work_total"] = workTotal.cents;
    work->fields["work_totalprint"] = workTotalPrint.cents;
    work->fields["work_processes"] = tasksBits;
    return true;
}

void WorkWidget::tasks2Forms(int workId)
{
    taskF->setMatchValue(workId);
    for (int row = 0; row < taskF->rowCount(); row++){
        Task *task = (Task *) taskF->data(taskF->index(row, 0), PPLib::entityRole).toInt();
        int process = task->fields["task_process"].toInt();
        taskMap[process]->entity2Form(task);
    }
}

bool WorkWidget::forms2Tasks(Entity *work)
{
    Money totalWork, totalPrintWork;

    taskF->setMatchValue(work->getId());
    for (int row = 0; row < taskF->rowCount(); row++){
        Task *task = (Task *) taskF->data(taskF->index(row, 0), PPLib::entityRole).toInt();
        int process = task->fields["task_process"].toInt();
        if (!taskMap[process]->form2Entity(task)){
            return false;
        }
        Money totalTask(task->fields["task_total"]);

        totalWork = totalWork + totalTask;
        if (process == mainProcess){
            totalPrintWork = totalTask;
        }
    }

    work->fields["work_total"] = totalWork.cents;
    work->fields["work_totalprint"] = totalPrintWork.cents;
    return true;
}

bool WorkWidget::delTasksFromWork(Entity *work)
{
    taskF->setMatchValue(work->getId());
    for (; taskF->rowCount() != 0;){

        if (!taskF->setData(taskF->index(0, 0), false, PPLib::delRole)) {
            return false;
        }
    }
    return true;
}

void WorkWidget::clearTasks()
{
    foreach (TaskWidget *taskW, tasks) {
        taskW->clear();
    }
}

void WorkWidget::registerTask(bool check){
    TaskWidget * taskWidget = (TaskWidget *)sender();
    if (check){
        connect(taskWidget, SIGNAL(totalChanged()), this, SLOT(totalChanged()));
        tasks.append(taskWidget);
        taskWidget->setWidgets(sizeW, priceTypeW, companyW, mainWidget);
        return;
    }
    tasks.removeOne(taskWidget);
    disconnect(taskWidget, SIGNAL(totalChanged()), this, SLOT(totalChanged()));
}


void WorkWidget::calcWorkTotal()
{
    workTotal = Money(0);
    foreach (TaskWidget *taskW, tasks) {
        workTotal = workTotal + taskW->getTotal();
    }
    workTotalPrint = taskMap[mainProcess]->getTotal();
}

void WorkWidget::resetId()
{
    for (int row = 0; row < workModel->rowCount(); row++){
        QModelIndex indx = workModel->index(row, 0);
        Work *work = (Work *) workModel->data(indx, PPLib::entityRole).toInt();

        int oldWorkId = work->getId();

        work->fields["work_id"] = idCounter;
        work->fields["work_status"] = Task::PREPARE;

        taskF->setMatchValue(oldWorkId);
        for (int row = 0; row < taskF->rowCount(); row++){
            QModelIndex indx = taskF->index(row, 0);
            Task *task = (Task *) taskF->data(indx, PPLib::entityRole).toInt();
            task->fields["work_id"] = idCounter;
            task->fields.remove("task_id");
            task->fields["task_status"] = Task::PREPARE;
        }

        idCounter -= 1;
    }
}
