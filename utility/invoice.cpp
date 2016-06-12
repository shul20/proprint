#include "invoice.h"
#include "ui_invoice.h"
#include <QDebug>

Invoice::Invoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Invoice)
{
    ui->setupUi(this);

    ui->isCache->setChecked(true);
    on_isCache_toggled(true);
}

Invoice::~Invoice()
{
    delete ui;
}

bool Invoice::isChache()
{
    return ui->isCache->isChecked();
}

const QString Invoice::getInvoice()
{
    return ui->invoice->text();
}

void Invoice::setInvoice(const QString &text)
{
    if (text == QString::fromUtf8("наличные")){
        setCache(true);
    } else {
        setCache(false);
        ui->invoice->setText(text);
    }
}

void Invoice::setCache(bool set)
{
    ui->isCache->setChecked(set);
    on_isCache_toggled(set);
}

void Invoice::on_isCache_toggled(bool checked)
{
    if (checked) {
        ui->invoice->setText(QString::fromUtf8("наличные"));
        ui->invoice->setDisabled(true);
        return;
    }

    ui->invoice->clear();
    ui->invoice->setEnabled(true);
}
