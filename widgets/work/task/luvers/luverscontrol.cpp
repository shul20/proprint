#include "luverscontrol.h"
#include "ui_luverscontrol.h"
#include <QDebug>

LuversControl::LuversControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LuversControl)
{
    ui->setupUi(this);
}

LuversControl::~LuversControl()
{
    delete ui;
}

void LuversControl::calculateAmount(){
    emit amountChanged(getAmount());
}

void LuversControl::sizeChanged(int width, int height, int amount){
    ui->perimetrAmount->lengthChanged((width + height) * 2, amount);
    ui->leftAmount->lengthChanged(height, amount);
    ui->upAmount->lengthChanged(width, amount);
    ui->rightAmount->lengthChanged(height, amount);
    ui->downAmount->lengthChanged(width, amount);
}

void LuversControl::getData(int &checks, qlonglong &values) {
    values = 0;
    checks = 0;

    if (ui->schemaBox->isChecked()){
        checks = 1;
    } else if (ui->perimetrBox->isChecked()) {
        checks |= (ui->perimetrAmount->getState() + 1) << 1;
        values = ui->perimetrAmount->getValue();
    } else {
        if (ui->leftBox->isChecked()) {
            checks |= (ui->leftAmount->getState() + 1) << 3;
            values = ui->leftAmount->getValue();
        }
        if (ui->upBox->isChecked()) {
            checks |= (ui->upAmount->getState() + 1) << 5;
            values |= ((qlonglong)ui->upAmount->getValue() << 16);
        }
        if (ui->rightBox->isChecked()) {
            checks |= (ui->rightAmount->getState() + 1) << 7;
            values |= ((qlonglong)ui->rightAmount->getValue() << 32);
        }
        if (ui->downBox->isChecked()) {
            checks |= (ui->downAmount->getState() + 1) << 9;
            values |= ((qlonglong)ui->downAmount->getValue() << 48);
        }
    }
}

void LuversControl::setData(int checks, qlonglong &values){
    if (checks == 1){
        ui->schemaBox->setChecked(true);
    } else if ((checks & 2) != 0) {
        ui->perimetrBox->setChecked(true);
        ui->perimetrAmount->setState(values, (checks & 4));
    } else {
        int leftValue = (values & 0xFFFF);
        int upValue = (values & 0xFFFF0000) >> 16;
        int rightValue = (values & 0xFFFF00000000) >> 32;
        int downValue = (values & 0xFFFF000000000000) >> 48;

     if ((checks & 8) != 0) {
        ui->leftBox->setChecked(true);
        ui->leftAmount->setState(leftValue, (checks & 16));
     }
     if ((checks & 32) != 0) {
        ui->upBox->setChecked(true);
        ui->upAmount->setState(upValue, (checks & 64));
     }
     if ((checks & 128) != 0) {
        ui->rightBox->setChecked(true);
        ui->rightAmount->setState(rightValue, (checks & 256));
     }
     if ((checks & 512) != 0) {
        ui->downBox->setChecked(true);
        ui->downAmount->setState(downValue, (checks & 1024));
     }
    }
}


void LuversControl::on_perimetrBox_toggled(bool check){
    if (check){
        ui->schemaBox->setChecked(false);
        resetCustomCheck();
        connect(ui->perimetrAmount, SIGNAL(amountChanged()), this, SLOT(calculateAmount()));
    }
}

void LuversControl::on_schemaBox_toggled(bool check){
    if (check){
        ui->perimetrBox->setChecked(false);
        resetCustomCheck();
    }
}

void LuversControl::on_upBox_toggled(bool check) {
    toggled(ui->upAmount, check);
}

void LuversControl::on_rightBox_toggled(bool check){
    toggled(ui->rightAmount, check);
}

void LuversControl::on_downBox_toggled(bool check){
    toggled(ui->downAmount, check);
}

void LuversControl::on_leftBox_toggled(bool check){
    toggled(ui->leftAmount, check);
}

void LuversControl::toggled(LuversAmount *block, bool check){
    if (check){
        ui->perimetrBox->setChecked(false);
        ui->schemaBox->setChecked(false);
        connect(block, SIGNAL(amountChanged()), this, SLOT(calculateAmount()));
    }
}

void LuversControl::resetCustomCheck(){
    ui->leftBox->setChecked(false);
    ui->upBox->setChecked(false);
    ui->rightBox->setChecked(false);
    ui->downBox->setChecked(false);
}

void LuversControl::clear(){
    ui->perimetrBox->setChecked(false);
    ui->schemaBox->setChecked(false);
    resetCustomCheck();
}

int LuversControl::getAmount()
{
    return  ui->perimetrAmount->amount +
            ui->leftAmount->amount +
            ui->upAmount->amount +
            ui->rightAmount->amount +
            ui->downAmount->amount;
}
