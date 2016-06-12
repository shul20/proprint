#include "luverstask.h"
#include "ui_luverstask.h"
#include "models/productmodel.h"
#include "utility/process.h"

LuversTask::LuversTask(QWidget *parent) :
    TaskWidget(parent, Process::LUVERS),
    ui(new Ui::LuversTask), amount(0)
{
    ui->setupUi(this);
    priceField = ui->price;

    connect(ui->product, SIGNAL(materialChanged(Stock*)),
            this, SLOT(stockChanged(Stock*)));

    connect(ui->control, SIGNAL(amountChanged(int)),
            this, SLOT(amountChanged(int)));

    connect(ui->totalInput, SIGNAL(textEdited(QString)), this,
            SLOT(totalInput(QString)));

    ui->product->setup(true, true, false, false, false);

}

LuversTask::~LuversTask(){
    delete ui;
}

void LuversTask::clear()
{
    ui->control->clear();
    total = Money(0);
    ui->totalInput->setMoney(total);
    emit totalChanged();
    ui->checkGroup->setChecked(false);
}

bool LuversTask::form2Entity(Entity *task)
{
    int checks;
    qlonglong values;
    ui->control->getData(checks, values);
    Process *process = Process::getInstance();

    task->fields["task_process"] = processId;
    task->fields["task_weight"] = process->getWeight(processId);

    Stock *stock = ui->product->getMaterial();
    if (stock == NULL) {
        return false;
    }
    task->fields["product_id"] = stock->fields["product_id"];
    task->fields["stock_id"] = stock->getId();

    task->fields["task_total"] = ui->totalInput->getMoney().cents;
    task->fields["task_lval_1"] = values;
    task->fields["task_lval_2"] = amount;
    task->fields["task_ival_3"] = checks;

    return true;
}

void LuversTask::entity2Form(Entity *task){
    ui->product->blockSignals(true);
    ui->control->blockSignals(true);

    Stock *stock = ui->product->getMaterial();
    if (stock == NULL) {
        ui->product->setRole(ProductModel::processRole, processId);
    }
    total = Money(task->fields["task_total"]);
    total.isCustom = true;


    productId = task->fields["product_id"].toInt();
    stockId = task->fields["stock_id"].toInt();
    ui->product->setMaterial(productId, stockId);

    int checks = task->fields["task_ival_3"].toInt();
    qlonglong values = task->fields["task_lval_1"].toLongLong();
    ui->control->setData(checks, values);

    ui->checkGroup->setChecked(true);
}


void LuversTask::amountChanged(int amount){
    this->amount = amount;
    ui->luversAmount->setValue(amount);
    calculateTotal();
}

void LuversTask::on_checkGroup_toggled(bool check){
    if (check) {
        Stock *stock = ui->product->getMaterial();
        if (stock == NULL) {
            ui->product->blockSignals(true);
            ui->product->setRole(ProductModel::processRole, processId);
            stock = ui->product->getMaterial();
        }

        emit setCheck(true);
        initWidget(stock);
        ui->product->blockSignals(false);
        ui->control->blockSignals(false);

    } else {
        clear();
        disconnectWidgets();
        emit setCheck(false);
    }
}

void LuversTask::calculateSize(){
    if (!ui->checkGroup->isChecked()){
        return;
    }

    ui->control->sizeChanged(sizeW->getWidth(), sizeW->getHeight(),
                             sizeW->getAmount());
    calculateTotal();
}

void LuversTask::calculateTotal()
{
    totalByPrice = price * (amount * 100);
    setTotal(ui->totalInput);
}

void LuversTask::initWidget(Stock *stock)
{
    ui->control->sizeChanged(sizeW->getWidth(), sizeW->getHeight(),
                             sizeW->getAmount());
    amount = ui->control->getAmount();
    ui->luversAmount->setValue(amount);
    loadStockVar(stock);
    calculatePrice();
    calculateTotal();
}



