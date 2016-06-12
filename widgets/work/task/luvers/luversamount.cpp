#include "luversamount.h"
#include "ui_luversamount.h"
#include <QDebug>

LuversAmount::LuversAmount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LuversAmount),
    length(0), items(0), value(0), amount(0)
{
    ui->setupUi(this);
}

LuversAmount::~LuversAmount()
{
    delete ui;
}

void LuversAmount::changeEvent(QEvent *event){
    if (!isEnabled()){
        ui->value->setValue(0);
        ui->amount->setCheckState(Qt::Checked);
        amount = 0;
        emit amountChanged();
    }
}

void LuversAmount::lengthChanged(qlonglong length, int items){
    this->length = length;
    this->items = items;
    calculateAmount();
}

void LuversAmount::setState(int value, int state){
    ui->value->setValue(value);
    if (state == 0) {
        ui->step->setChecked(Qt::Checked);
    }
}

int LuversAmount::getState(){
    return ((int)ui->amount->isChecked()) << 1;
}

int LuversAmount::getValue(){
    return value;
}

void LuversAmount::on_value_valueChanged(int value){
    this->value = value;
    calculateAmount();
}

void LuversAmount::on_amount_toggled(bool checked){
    calculateAmount();
}


void LuversAmount::calculateAmount(){
    int value = ui->value->value();
    if (value == 0 || length == 0 || items == 0){
        amount = 0;
    } else if (ui->amount->isChecked()){
        amount = value * items;
    } else if (ui->step->isChecked()) {
        amount = (length / value) * items;
    }
    emit amountChanged();
}


void LuversAmount::on_step_toggled(bool checked){
    calculateAmount();
}
