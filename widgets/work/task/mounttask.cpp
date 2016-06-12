#include "mounttask.h"
#include "ui_mounttask.h"
#include "utility/process.h"
#include "models/productmodel.h"

MountTask::MountTask(QWidget *parent) :
    TaskWidget(parent, Process::MOUNT),
    ui(new Ui::MountTask)
{
    ui->setupUi(this);
    priceField = ui->price;

    connect(ui->product, SIGNAL(materialChanged(Stock*)),
            this, SLOT(stockChanged(Stock*)));

    connect(ui->product, SIGNAL(paramChanged(Stock*)),
            ui->stockInfo, SLOT(stockChanged(Stock*)));

    connect(ui->totalInput, SIGNAL(textEdited(QString)), this,
            SLOT(totalInput(QString)));

    ui->product->setup(true, true, true, false, false);

}

MountTask::~MountTask()
{
    delete ui;
}

void MountTask::clear()
{
    total = Money(0);
    ui->totalInput->setMoney(total);
    emit totalChanged();
    ui->price->setMoney(Money(0));
    ui->checkGroup->setChecked(false);
}

bool MountTask::form2Entity(Entity *task)
{
    Process *process = Process::getInstance();

    task->fields["task_process"] = processId;
    task->fields["task_weight"] = process->getWeight(processId);

    Stock *stock = ui->product->getParam();
    if (stock == NULL) {
        return false;
    }
    task->fields["product_id"] = stock->fields["product_id"];
    task->fields["stock_id"] = stock->getId();

    task->fields["task_total"] = ui->totalInput->getMoney().cents;

    return true;
}

void MountTask::entity2Form(Entity *task)
{
    ui->product->blockSignals(true);

    Stock *stock = ui->product->getMaterial();
    if (stock == NULL) {
        ui->product->setRole(ProductModel::processRole, processId);
    }
    total = Money(task->fields["task_total"]);
    total.isCustom = true;

    int stockId = task->fields["stock_id"].toInt();
    int productId = task->fields["product_id"].toInt();
    ui->product->setParam(productId, stockId);

    ui->checkGroup->setChecked(true);
}

void MountTask::on_checkGroup_toggled(bool check)
{
    if (check) {
        Stock *stock = ui->product->getParam();
        if (stock == NULL) {
            ui->product->blockSignals(true);
            ui->product->setRole(ProductModel::processRole, processId);
            stock = ui->product->getMaterial();
        }

        emit setCheck(true);
        initWidget(stock);
        ui->product->blockSignals(false);

    } else {
        clear();
        disconnectWidgets();
        emit setCheck(false);
    }
}

void MountTask::calculateSize()
{
    if (!ui->checkGroup->isChecked()){
        return;
    }
    size = sizeW->getSize();
    ui->size->setText(size.toString());
    calculateTotal();
}

void MountTask::calculateTotal()
{
    totalByPrice = price * size;
    setTotal(ui->totalInput);
}

void MountTask::initWidget(Stock *stock)
{
    size = sizeW->getSize();
    ui->size->setText(size.toString());
    ui->stockInfo->stockChanged(stock);
    loadStockVar(stock);
    calculatePrice();
    calculateTotal();
}
