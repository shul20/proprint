#include "pockettask.h"
#include "ui_pockettask.h"
#include "utility/process.h"
#include "models/productmodel.h"

PocketTask::PocketTask(QWidget *parent) :
    TaskWidget(parent, Process::POCKET),
    ui(new Ui::PocketTask), isTop(0), isBottom(0)
{
    ui->setupUi(this);
    priceField = ui->price;

    connect(ui->product, SIGNAL(productChanged(Product*)),
            this, SLOT(productChanged(Product*)));
    connect(ui->totalInput, SIGNAL(textEdited(QString)), this,
            SLOT(totalInput(QString)));

    ui->product->setup(true, false, false, false, false);
}

PocketTask::~PocketTask()
{
    delete ui;
}

void PocketTask::clear()
{
    ui->top->setValue(0);
    ui->bottom->setValue(0);
    total = Money(0);
    ui->totalInput->setMoney(total);
    emit totalChanged();
    ui->price->setMoney(Money(0));
    ui->checkGroup->setChecked(false);
}

bool PocketTask::form2Entity(Entity *task)
{
    Process *process = Process::getInstance();

    task->fields["task_process"] = processId;
    task->fields["task_weight"] = process->getWeight(processId);


    Product *product = ui->product->getProduct();
    if (product == NULL) {
        return false;
    }

    task->fields["product_id"] = product->getId();
    task->fields["task_total"] = ui->totalInput->getMoney().cents;
    task->fields["task_ival_3"] = ui->top->value();
    task->fields["task_ival_4"] = ui->bottom->value();

    return true;
}

void PocketTask::entity2Form(Entity *task)
{
    ui->product->blockSignals(true);

    Product *product = ui->product->getProduct();
    if (product == NULL) {
        ui->product->setRole(ProductModel::processRole, processId);
    }

    total = Money(task->fields["task_total"]);
    total.isCustom = true;

    int productId = task->fields["product_id"].toInt();
    ui->product->setProduct(productId);

    ui->top->blockSignals(true);
    ui->bottom->blockSignals(true);

    ui->top->setValue(task->fields["task_ival_3"].toInt());
    ui->bottom->setValue(task->fields["task_ival_4"].toInt());

    ui->top->blockSignals(false);
    ui->bottom->blockSignals(false);

    ui->checkGroup->setChecked(true);
}

void PocketTask::on_checkGroup_toggled(bool check)
{
    if (check) {
        Product *product = ui->product->getProduct();
        if (product == NULL) {
            ui->product->blockSignals(true);
            ui->product->setRole(ProductModel::processRole, processId);
            product = ui->product->getProduct();
        }

        emit setCheck(true);
        initWidget(product);
        ui->product->blockSignals(false);

    } else {
        clear();
        disconnectWidgets();
        emit setCheck(false);
    }
}

void PocketTask::calculateSize()
{
    calculateTotal();
}

void PocketTask::calculateTotal()
{
    int a = (ui->top->value() > 0) + (ui->bottom->value() > 0);

    qlonglong totalWidth = sizeW->getWidth() * a * sizeW->getAmount();
    totalByPrice = (price * (totalWidth / 10));
    setTotal(ui->totalInput);
}

void PocketTask::initWidget(Product *product)
{
    productId = product->getId();
    calculatePrice();
    calculateTotal();
}

void PocketTask::on_top_valueChanged(int value){
    calculateTotal();
}

void PocketTask::on_bottom_valueChanged(int value){
    calculateTotal();
}
