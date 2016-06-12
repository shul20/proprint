#include "laminattask.h"
#include "ui_laminattask.h"
#include "models/productmodel.h"
#include "utility/process.h"
#include "models/equip/equipmentmodel.h"
#include "entity/order/task.h"
#include "widgets/work/task/sizewidget.h"

LaminatTask::LaminatTask(QWidget *parent) :
    TaskWidget(parent, Process::LAMINAT),
    ui(new Ui::LaminatTask)
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

    ui->equip->setup(true, false, false, false);
    ui->equip->setEquipRole(EquipmentModel::processRole, processId);
}

LaminatTask::~LaminatTask()
{
    delete ui;
}

Entity *LaminatTask::getStock()
{
    return ui->product->getMaterial();
}

Entity *LaminatTask::getEquip()
{
    return ui->equip->getEquip();
}

void LaminatTask::clear()
{
    total = Money(0);
    ui->totalInput->setMoney(total);
    emit totalChanged();
    ui->price->setMoney(Money(0));
    ui->checkGroup->setChecked(false);
}

bool LaminatTask::form2Entity(Entity *task)
{
    Process *process = Process::getInstance();

    task->fields["task_process"] = processId;
    task->fields["task_weight"] = process->getWeight(processId);

    task->fields["equipment_id"] = ui->equip->getEquip()->getId();

    Stock *stock = ui->product->getParam();
    if (stock == NULL) {
        return false;
    }
    task->fields["product_id"] = stock->fields["product_id"];
    task->fields["stock_id"] = stock->getId();

    task->fields["task_total"] = ui->totalInput->getMoney().cents;

    return true;
}

void LaminatTask::entity2Form(Entity *task)
{
    ui->product->blockSignals(true);

    Stock *stock = ui->product->getParam();
    if (stock == NULL) {
        ui->product->setRole(ProductModel::processRole, processId);
    }

    total = Money(task->fields["task_total"]);
    total.isCustom = true;

    ui->equip->setEquip(task->fields["equipment_id"].toInt());

    int productId = task->fields["product_id"].toInt();
    int stockId = task->fields["stock_id"].toInt();
    ui->product->setParam(productId, stockId);

    ui->checkGroup->setChecked(true);
}

void LaminatTask::showEvent(QShowEvent *)
{
}

void LaminatTask::calculateSize()
{
    if (!ui->checkGroup->isChecked()){
        return;
    }
    size = sizeW->getSize();
    ui->size->setText(size.toString());
    calculateTotal();
}

void LaminatTask::calculateTotal()
{
    totalByPrice = price * size;
    setTotal(ui->totalInput);
}

void LaminatTask::initWidget(Stock *stock)
{
    if (stock == NULL) {
        return;
    }
    size = sizeW->getSize();
    ui->size->setText(size.toString());
    ui->stockInfo->stockChanged(stock);
    loadStockVar(stock);
    calculatePrice();
    calculateTotal();
}

void LaminatTask::on_checkGroup_toggled(bool check)
{
    if (check) {
        Stock *stock = ui->product->getParam();
        if (stock == NULL) {
            ui->product->blockSignals(true);
            ui->product->setRole(ProductModel::processRole, processId);
            stock = ui->product->getParam();
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
