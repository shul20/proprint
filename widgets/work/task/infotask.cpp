#include "infotask.h"
#include "ui_infotask.h"
#include "utility/process.h"
#include <QColor>
#include "widgets/work/task/printtask.h"

InfoTask::InfoTask(QWidget *parent, int processId, bool isTotal) :
    TaskWidget(parent, processId),
    ui(new Ui::InfoTask)
{
    ui->setupUi(this);
    ui->checkGroup->setTitle(objectName());
    ui->totalFrame->setHidden(!isTotal);

    connect(ui->totalInput, SIGNAL(textEdited(QString)), this,
            SLOT(totalInput(QString)));

}

InfoTask::~InfoTask()
{
    delete ui;
}

void InfoTask::clear()
{
    total = Money(0);
    ui->totalInput->setMoney(total);
    emit totalChanged();
    ui->info->clear();
    ui->checkGroup->setChecked(false);
}

bool InfoTask::form2Entity(Entity *task)
{
    Process *process = Process::getInstance();

    task->fields["task_process"] = processId;
    task->fields["task_weight"] = process->getWeight(processId);

    task->fields["task_info"] = ui->info->text();
    task->fields["task_total"] = ui->totalInput->getMoney().cents;

    if (processId == Process::COLORPROOF){
        PrintTask *print = (PrintTask *) mainW;

        Entity *equip = print->getEquip();
        ProductWidget *productW = print->getProductW();
        Entity *stock = productW->getMaterial();

        if (stock == NULL || equip == NULL) {
            return false;
        }

        task->fields["equipment_id"] = equip->getId();

        task->fields["product_id"] = stock->fields["product_id"];
        task->fields["task_stock"] = stock->fields["stock_name"];
        task->fields["stock_id"] = stock->getId();

        task->fields["task_ival_3"] = print->getPrintType();
    }

    return true;
}

void InfoTask::entity2Form(Entity *task)
{
    total = Money(task->fields["task_total"]);
    total.isCustom = true;

    ui->info->setText(task->fields["task_info"].toString());
    ui->checkGroup->setChecked(true);
    calculateTotal();
}

void InfoTask::on_checkGroup_toggled(bool check)
{
    if (!check) {
        clear();
    }
    emit setCheck(check);
    disconnectWidgets();
}

void InfoTask::calculateTotal()
{
    setTotal(ui->totalInput);
}

void InfoTask::calculateSize()
{
}
