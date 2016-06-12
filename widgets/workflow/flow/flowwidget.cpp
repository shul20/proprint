#include "flowwidget.h"
#include "entity/order/task.h"
#include "entity/order/taskview.h"
#include "models/modelmanager.h"

FlowWidget::FlowWidget(QWidget *parent) :
    QWidget(parent), status(1), task(NULL), isReady(false)
{}

void FlowWidget::set(Entity *order, Entity *work,
                     SortFilterModel *taskF, WorkModel *workModel)
{
    this->order = order;
    this->work = work;
    this->taskF = taskF;
    this->workModel = workModel;
}

void FlowWidget::startClicked()
{
    if (!checkWeight() || !checkEquipment()){
        return;
    }
    setStatus(Task::PROCESS);
}

void FlowWidget::pauseClicked()
{
    setStatus(Task::PAUSE);
}

void FlowWidget::readyClicked()
{
    setStatus(Task::READY);
}

void FlowWidget::setStatus(int status)
{
    groupBox->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    if (status == Task::PREPARE){
        startButton->setEnabled(true);
        pauseButton->setDisabled(true);
        readyButton->setDisabled(true);

    } else if (status == Task::PROCESS){
        startButton->setDisabled(true);
        pauseButton->setEnabled(true);
        readyButton->setEnabled(true);
        task->fields["task_status"] = status;
        work->fields["work_status"] = Task::PROCESS;

    } else if (status == Task::PAUSE){
        startButton->setEnabled(true);
        pauseButton->setDisabled(true);
        readyButton->setEnabled(true);
        task->fields["task_status"] = status;
        changeWorkStatus();

    } else if (status == Task::READY){
        startButton->setDisabled(true);
        pauseButton->setDisabled(true);
        readyButton->setDisabled(true);
        groupBox->setAttribute(Qt::WA_TransparentForMouseEvents);
        task->fields["task_status"] = status;
        changeWorkStatus();
    }
    groupBox->setStyleSheet(
                QString("QGroupBox#groupBox {background: %1}").
                arg(Task::statusesColor[status].name()));

}

void FlowWidget::disableFocus()
{
     startButton->setFocusPolicy(Qt::NoFocus);
     pauseButton->setFocusPolicy(Qt::NoFocus);
     readyButton->setFocusPolicy(Qt::NoFocus);

}

void FlowWidget::changeWorkStatus()
{
   int statusBits = 0;

    for (int row = 0; row < taskF->rowCount(); row++) {
        QModelIndex index = taskF->index(row, 0);
        Task *task = (Task *) taskF->data(index, PPLib::entityRole).toInt();
        int status = task->fields["task_status"].toInt();
        statusBits |= (1 << status);
    }

    if ( statusBits == 1) {
        work->fields["work_status"] = Task::READY;

    } else if (((statusBits >> Task::PROCESS) & 1) == 1) {
        work->fields["work_status"] = Task::PROCESS;

    } else if (((statusBits >> Task::PAUSE) & 1) == 1) {
        work->fields["work_status"] = Task::PAUSE;

    } else {
        work->fields["work_status"] = Task::PREPARE;
    }
    workModel->reset();
}

bool FlowWidget::checkWeight()
{
    int checkedWeight = task->fields["task_weight"].toInt();
    for (int row = 0; row < taskF->rowCount(); row++) {
        QModelIndex index = taskF->index(row, 0);
        Task *task = (Task *) taskF->data(index, PPLib::entityRole).toInt();
        int weight = task->fields["task_weight"].toInt();
        int status = task->fields["task_status"].toInt();

        if (status != Task::READY && weight < checkedWeight) {
            QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
               QString::fromUtf8("Выполните более важную задачу"), QMessageBox::Ok);
            return false;
        }
    }
    return true;
}

bool FlowWidget::checkEquipment()
{
    int equipId = task->fields["equipment_id"].toInt();
    if (equipId == 0) {
        return true;
    }
    ModelManager *models = ModelManager::getInstance();

    if (models->getTasksPrintModel()->isPrinterBusy(equipId)) {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
           QString::fromUtf8("Принтер уже выполняет заказ"), QMessageBox::Ok);

        return false;
    }

    return true;
}
