#include "printtask.h"
#include "ui_printtask.h"
#include <QFileDialog>
#include "models/productmodel.h"
#include "utility/process.h"
#include "models/equip/equipmentmodel.h"
#include "entity/order/task.h"
#include "widgets/work/task/sizewidget.h"

PrintTask::PrintTask(QWidget *parent) :
    TaskWidget(parent, Process::PRINT),
    ui(new Ui::PrintTask), sideAmount(1)
{
    ui->setupUi(this);
    priceField = ui->price;

    connect(ui->product, SIGNAL(materialChanged(Stock*)),
            this, SLOT(stockChanged(Stock*)));

    connect(ui->product, SIGNAL(paramChanged(Stock*)),
            ui->stockInfo, SLOT(stockChanged(Stock*)));

    ui->product->setup(true, true, true, false, false);

    ui->equip->setup(true, false, false, false);
    ui->equip->setEquipRole(EquipmentModel::processRole, processId);

    ui->printType->blockSignals(true);
    for (int i = 0; i < Task::typeList.length(); i++){
        ui->printType->addItem(Task::typeList[i], i);
    }
    ui->printType->blockSignals(false);

}

PrintTask::~PrintTask()
{
    delete ui;
}

ProductWidget *PrintTask::getProductW()
{
    return ui->product;
}

Entity *PrintTask::getEquip()
{
    return ui->equip->getEquip();
}

int PrintTask::getPrintType()
{
    return ui->printType->currentIndex();
}

void PrintTask::clear()
{
    ui->path->clear();
    ui->priceForOne->setMoney(Money(0));
    ui->over->setMoney(Money(0));
}

bool PrintTask::form2Entity(Entity *task)
{
    QString path = ui->path->text();

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

    task->fields["task_info"] = path;
    task->fields["task_ival_3"] = ui->printType->currentIndex();
    task->fields["task_total"] = total.cents;
    task->fields["task_lval_1"] = ui->priceForOne->getMoney().cents;
    task->fields["task_lval_2"] = ui->over->getMoney().cents;

    return true;
}

void PrintTask::entity2Form(Entity *task)
{
    ui->equip->setEquip(task->fields["equipment_id"].toInt());

    stockId = task->fields["stock_id"].toInt();
    productId = task->fields["product_id"].toInt();
    ui->product->setParam(productId, stockId);

    ui->path->setText(task->fields["task_info"].toString());
    ui->printType->setCurrentIndex(task->fields["task_ival_3"].toInt());
    ui->priceForOne->setMoney(Money(task->fields["task_lval_1"]));
    ui->over->setMoney(task->fields["task_lval_2"]);

    Money saveTotal = Money(task->fields["task_total"]);
    if (saveTotal != totalByPrice) {
        saveTotal.isCustom = true;
    }
    ui->total->setMoney(saveTotal);

}

void PrintTask::showEvent(QShowEvent *)
{
    emit setCheck(true);
    disconnect(sizeW, SIGNAL(sizeChanged()), this, SLOT(sizeChanged()));
    ui->product->blockSignals(true);
    ui->product->setRole(ProductModel::processRole, processId);
    Stock *stock = ui->product->getMaterial();
    if (stock == NULL) {
        return;
    }
    initWidget(stock);
    connect(sizeW, SIGNAL(sizeChanged()), this, SLOT(sizeChanged()));
    ui->product->blockSignals(false);
}

void PrintTask::initWidget(Stock *stock)
{
    size = sizeW->getSize();
    ui->size->setText(size.toString());
    ui->stockInfo->stockChanged(stock);
    loadStockVar(stock);
    calculatePrice();
    calculateTotal();
}

void PrintTask::calculateSize()
{
    size = sizeW->getSize();
    size = size * sideAmount;
    ui->size->setText(size.toString());
    calculateTotal();
}

void PrintTask::on_printType_currentIndexChanged(int index)
{
    if (index == Task::DOUBLESIDE) {
            sideAmount = 2;
        } else {
            sideAmount = 1;
        }
        calculateSize();
}

void PrintTask::on_pathButton_clicked()
{
    QString path = QFileDialog:: QFileDialog::getOpenFileName(this,
                                 QString::fromUtf8("Выберите файл"), "", "");
    ui->path->setText(path);
}

void PrintTask::on_priceForOne_textChanged(const QString &value)
{
    priceForOne = Money(value);
    if (priceForOne == 0) {
        ui->price->setEnabled(true);
    } else {
        ui->price->setDisabled(true);
    }
    calculateTotal();
}

void PrintTask::calculateTotal()
{
    totalByPrice = price * size;
    if (priceForOne == 0) {
         total = totalByPrice + priceOver;
     } else {
         total = priceForOne * (sizeW->getAmount() * 100) + priceOver;
     }

     if (total != totalByPrice) {
         total.isCustom = true;
     }
     ui->total->setMoney(total);

     emit totalChanged();
}

void PrintTask::on_over_textChanged(const QString &value)
{
    priceOver = Money(value);

    calculateTotal();
}
