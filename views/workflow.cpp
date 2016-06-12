#include "workflow.h"
#include "ui_workflow.h"
#include "models/modelmanager.h"
#include "models/order/tasksprintm.h"
#include "entity/order/task.h"
#include "models/modelmanager.h"
#include "utility/process.h"

WorkFlow::WorkFlow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkFlow)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Рабочий процесс"));

    ui->start->setup(this, ui->info);

    ModelManager *models = ModelManager::getInstance();
    tasksPrintM = models->getTasksPrintModel();
    tasksPostM = models->getTasksPostModel();

    taskQuery.prepare(TaskView::allByStatus);
    taskQuery.bindValue(":status", Task::PROCESS);
    refreshTasks();
    ui->print->setup(tasksPrintM, QString::fromUtf8("Заказы в печати"), ui->info);
    ui->postPrint->setup(tasksPostM, QString::fromUtf8("Заказы в постпечати"), ui->info);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshTasks()));
    timer->start(models->getVarModel()->getVar(Var::TREFRESH).toInt());

    ui->ready->setup(ui->info);

    ui->mainSpliiter->setStretchFactor(0, 5);
    ui->mainSpliiter->setStretchFactor(1, 1);
    QString stylesheet = "QSplitter::handle{background: gray; width: 2px; height: 4px;}";
    ui->splitterOrders->setStyleSheet(stylesheet);
}

WorkFlow::~WorkFlow()
{
    delete ui;
}

void WorkFlow::refreshTasks()
{
    if (!taskQuery.exec()){
        return;
    }
    tasksPrintM->clearTasks();
    tasksPostM->clearTasks();

    QMap<int, Entity *> &tasksPrint = tasksPrintM->getTasks();
    QList<Entity *> &tasksPostPrint = tasksPostM->getTasks();

    for(;taskQuery.next();){
        Entity *task = new TaskView();
        task->setFields(taskQuery.record());
        int equipId = task->fields["equipment_id"].toInt();
        int processId = task->fields["task_process"].toInt();

        if (processId == Process::PRINT || processId == Process::COLORPROOF) {
            tasksPrint[equipId] = task;
        } else {
            tasksPostPrint.append(task);
        }
    }

    tasksPrintM->refresh();
    tasksPostM->refresh();
}

void WorkFlow::on_viewStartButton_clicked()
{
    timer->stop();

    ui->start->show();
    ui->print->hide();
    ui->postPrint->hide();
    ui->ready->hide();

    QList<int> sizes = QList<int>() << 100 <<  0 << 0 << 0;
    ui->splitterOrders->setSizes(sizes);

    for (int i = 0; i < ui->splitterOrders->count(); i++)
    {
        QSplitterHandle *hndl = ui->splitterOrders->handle(i);
        hndl->setEnabled(false);
    }
}

void WorkFlow::on_viewAllButton_clicked()
{
    ui->start->show();
    ui->print->show();
    ui->postPrint->show();
    ui->ready->show();

    QList<int> sizes = QList<int>() << 100 <<  100 << 100 << 100;
    ui->splitterOrders->setSizes(sizes);

    for (int i = 0; i < ui->splitterOrders->count(); i++)
    {
        QSplitterHandle *hndl = ui->splitterOrders->handle(i);
        hndl->setEnabled(true);
    }
    timer->start(timer->interval());
}

void WorkFlow::on_viewPrintButton_clicked()
{
    timer->stop();

    ui->start->hide();
    ui->print->show();
    ui->postPrint->hide();
    ui->ready->hide();

    QList<int> sizes = QList<int>() << 0 <<  100 << 0 << 0;
    ui->splitterOrders->setSizes(sizes);

    for (int i = 0; i < ui->splitterOrders->count(); i++)
    {
        QSplitterHandle *hndl = ui->splitterOrders->handle(i);
        hndl->setEnabled(false);
    }
    timer->start(timer->interval());
}

void WorkFlow::on_viewPostPrintButton_clicked()
{
    timer->stop();

    ui->start->hide();
    ui->print->hide();
    ui->postPrint->show();
    ui->ready->hide();

    QList<int> sizes = QList<int>() << 0 << 0 << 100 << 0;
    ui->splitterOrders->setSizes(sizes);

    for (int i = 0; i < ui->splitterOrders->count(); i++)
    {
        QSplitterHandle *hndl = ui->splitterOrders->handle(i);
        hndl->setEnabled(false);
    }
    timer->start(timer->interval());
}

void WorkFlow::on_readyButton_clicked()
{
    timer->stop();

    ui->start->hide();
    ui->print->hide();
    ui->postPrint->hide();
    ui->ready->show();

    QList<int> sizes = QList<int>() << 0 << 0 << 0 << 100;
    ui->splitterOrders->setSizes(sizes);

    for (int i = 0; i < ui->splitterOrders->count(); i++)
    {
        QSplitterHandle *hndl = ui->splitterOrders->handle(i);
        hndl->setEnabled(false);
    }
}
