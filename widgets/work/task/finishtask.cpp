#include "finishtask.h"
#include "ui_finishtask.h"
#include "utility/process.h"
#include "models/productmodel.h"

FinishTask::FinishTask(QWidget *parent) :
    TaskWidget(parent, Process::FINISHING),
    ui(new Ui::FinishTask)
{
    ui->setupUi(this);
    priceField = ui->price;

    connect(ui->product, SIGNAL(productChanged(Product*)),
            this, SLOT(productChanged(Product*)));
    connect(ui->totalInput, SIGNAL(textEdited(QString)), this,
            SLOT(totalInput(QString)));

    ui->product->setup(true, false, false, false, false);
}

FinishTask::~FinishTask()
{
    delete ui;
}

void FinishTask::clear()
{
    total = Money(0);
    ui->totalInput->setMoney(total);
    emit totalChanged();
    ui->price->setMoney(Money(0));
    ui->checkGroup->setChecked(false);
    ui->top->setChecked(false);
    ui->down->setChecked(false);
    ui->left->setChecked(false);
    ui->right->setChecked(false);
}

bool FinishTask::form2Entity(Entity *task)
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

    int checks = 0;
    checks |= (int) ui->top->isChecked();
    checks |= ((int) ui->down->isChecked()) << 1;
    checks |= ((int) ui->left->isChecked()) << 2;
    checks |= ((int) ui->right->isChecked()) << 3;

    task->fields["task_ival_3"] = checks;

    return true;
}

void FinishTask::entity2Form(Entity *task)
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

    int checks = task->fields["task_ival_3"].toInt();

    ui->top->blockSignals(true);
    ui->down->blockSignals(true);
    ui->left->blockSignals(true);
    ui->right->blockSignals(true);

    ui->top->setChecked((bool)(checks & 1));
    ui->down->setChecked((bool)((checks & 2) >> 1));
    ui->left->setChecked((bool)((checks & 4) >> 2));
    ui->right->setChecked((bool)((checks & 8) >> 3));

    ui->top->blockSignals(false);
    ui->down->blockSignals(false);
    ui->left->blockSignals(false);
    ui->right->blockSignals(false);

    ui->checkGroup->setChecked(true);
}

void FinishTask::on_checkGroup_toggled(bool check)
{
    if (check) {
        Product *product = ui->product->getProduct();
        if (product == NULL) {
            ui->product->blockSignals(true);
            ui->product->setRole(ProductModel::processRole, processId);
            product = ui->product->getProduct();
        }

        emit setCheck(true);
        productId = product->getId();
        calculatePrice();
        calculateTotal();

        ui->product->blockSignals(false);

    } else {
        clear();
        disconnectWidgets();
        emit setCheck(false);
    }
}

void FinishTask::calculateSize()
{
    calculateTotal();
}

void FinishTask::calculateTotal()
{
    int top = ui->top->isChecked();
    int down = ui->down->isChecked();
    int left = ui->left->isChecked();
    int right = ui->right->isChecked();

    int length = sizeW->getWidth() * (top + down) +
            sizeW->getHeight() * (left + right);

    totalByPrice = (price * (length * 10)) * sizeW->getAmount();
    setTotal(ui->totalInput);
}

void FinishTask::on_left_toggled(bool checked){
    calculateTotal();
}

void FinishTask::on_right_toggled(bool checked){
    calculateTotal();
}

void FinishTask::on_top_toggled(bool checked){
    calculateTotal();
}

void FinishTask::on_down_toggled(bool checked){
    calculateTotal();
}
