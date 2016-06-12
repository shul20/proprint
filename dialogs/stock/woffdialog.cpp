#include "woffdialog.h"
#include "ui_woffdialog.h"
#include "entity/stock/material.h"
#include "entity/client.h"
#include "entity/stock/woff.h"
#include "entity/finance/payment.h"
#include "mainwindow.h"
#include "widgets/popupwindow.h"
#include <QDateTime>
#include "models/finance/paymodel.h"
#include "models/modelmanager.h"
#include "models/sortfiltermodel.h"

WoffDialog::WoffDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WoffDialog)
{
    ui->setupUi(this);
    labels << QString::fromUtf8("мм")
           << QString::fromUtf8("кв. м")
           << QString::fromUtf8("л/шт.")
           << QString::fromUtf8("рулонов")
           << QString::fromUtf8("листов")
           << QString::fromUtf8("единиц");

    PayModel *payModel = ModelManager::getInstance()->getPayModel();
    SortFilterModel *payModelF = new SortFilterModel(this);
    payModelF->setSourceModel(payModel);
    payModelF->setFilterRole(PayModel::stockRole);
    ui->pays->setModel(payModelF);
    payModelF->setMatchValue(true);

}

WoffDialog::~WoffDialog()
{
    delete ui;
}

void WoffDialog::setModel(QAbstractItemModel *model, Entity *view)
{
    this->view = view;
    entity2Form(view);
}

void WoffDialog::on_saveButton_clicked()
{
    QString message;
    Woff *woff = new Woff();

    MainWindow::db->transaction();
    if (!form2Entity(woff, message) ||
        !updatePayment(woff, message) ||
        !woff->update()) {

        if (message.length() == 0){
            message = Woff::lastError;
        }
        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), message);
        delete woff;
        MainWindow::db->rollback();
        return;
    }

    delete woff;
    MainWindow::db->commit();

    PopUpWindow *popMessage = new PopUpWindow(this, QString::fromUtf8("материал списан"));
    popMessage->show();
    QTimer::singleShot(700, popMessage, SLOT(deleteLater()));

    Stock *stock = (Stock *) view;
    stock->retrive(stock->getId(), false);

    QDialog::accept();
}

void WoffDialog::on_exitButton_clicked()
{
    QDialog::reject();
}

void WoffDialog::on_rollSize_textEdited(const QString &arg1)
{
    ui->rollAmount->setValue(0);
}

void WoffDialog::on_rollAmount_valueChanged(int arg1)
{
    ui->rollSize->setText("0.00");
}

bool WoffDialog::form2Entity(Entity *woff, QString &message)
{
    QString info = ui->info->toPlainText();
    int stockType = view->fields["stock_type"].toInt();
    int param = view->fields["stock_param"].toInt();

    Size rollSize(param, PPLib::rollLength);

    if (stockType == Stock::ROLL){
        Size woffSize = Size(ui->rollSize->text());
        int woffAmount = ui->rollAmount->value();

        if (woffAmount != 0){
            woffSize = rollSize.value * woffAmount;
        }

        woff->fields["woff_amount"] = woffSize.value;

    } else if (stockType == Stock::LIST) {
        woff->fields["woff_amount"] = ui->itemAmount->value();

    } else if (stockType == Stock::ITEM) {
        int woffAmount = ui->itemAmount->value();
        woff->fields["woff_amount"] = woffAmount * param;
    }

    if (info.length() == 0 || woff->fields["woff_amount"].toInt() == 0) {
        message = QString::fromUtf8("Заполните форму");
        return false;
    }

    woff->fields["woff_info"] = info;
    woff->fields["stock_id"] = view->getId();
    woff->fields["client_id"] = Client::currentClient->getId();
    woff->fields["woff_date"] = QDateTime::currentDateTime();

    return true;
}

bool WoffDialog::updatePayment(Entity *woff, QString &message)
{
    Money payTotal = ui->pay->getMoney();
    if (payTotal == 0) {
        return true;
    }
    Pay *pay = (Pay *) ui->pays->itemData(ui->pays->currentIndex(), PPLib::entityRole).toInt();
    if (pay == NULL) {
        return false;
    }
    Payment *payment = new Payment();
    payment->fields["client_id"] = Client::currentClient->getId();
    payment->fields["payment_total"] = payTotal.cents;
    payment->fields["pay_id"] = pay->getId();
    payment->fields["payment_field_id"] = woff->getId();
    payment->fields["payment_iscache"] = ui->invoice->isChache();
    payment->fields["payment_invoice"] = ui->invoice->getInvoice();
    payment->fields["payment_date"] = QDateTime::currentDateTime();

    QString info = QString("%1 %2 %3")
            .arg(ui->product->text(), ui->material->text(), ui->param->text());

    payment->fields["payment_info"] = info;

    if (!payment->update()) {
        message = Payment::lastError;
        delete pay;
        return false;
    }

    woff->fields["payment_id"] = payment->getId();
    delete payment;
    return true;
}

void WoffDialog::entity2Form(Entity *stock)
{
    Stock *stockView = (Stock *)stock;

    int stockType = stockView->fields["stock_type"].toInt();

    ui->product->setText(stockView->getProductName());
    ui->material->setText(stockView->fields["stock_name"].toString());
    int param = stockView->fields["stock_param"].toInt();
    ui->param->setValue(param);
    QVariant amount = stockView->fields["stock_amount"];

    if (stockType == Stock::ROLL){
        ui->parametrName->setText(labels[ROLL]);
        ui->parametrName1->setText(labels[ROLL]);
        ui->parametrUnit->setText(labels[MM]);
        ui->sizeUnit->setText(labels[M2]);

        Size rollSize(param, PPLib::rollLength);
        int rolls = qRound(amount.toReal() / (qreal)rollSize.value);

        ui->stockAmount->setValue(rolls);
        ui->stockSize->setText(Size(amount).toString());
        ui->woffList->hide();

    } else if (stockType == Stock::LIST) {
        ui->parametrName->setText(labels[LIST]);
        ui->parametrName1->setText(labels[LIST]);
        ui->parametrUnit->setText(labels[MM]);
        ui->woffRoll->hide();

        ui->stockAmount->setValue(amount.toInt());

    } else if (stockType == Stock::ITEM) {
        ui->parametrName->setText(labels[E]);
        ui->parametrName1->setText(labels[E]);
        ui->parametrUnit->setText(labels[L]);
        ui->sizeUnit->setText(labels[L]);
        ui->woffRoll->hide();

        ui->stockAmount->setValue(amount.toInt() / param);
        ui->stockSize->setText(amount.toString());
        ui->labelItem->setText(QString::fromUtf8("Количество"));
    }

    QPair<Money, Money> priceRange = Material::getPriceRange(stockView->getId());

    ui->priceMin->setMoney(priceRange.first);
    ui->priceMax->setMoney(priceRange.second);

    adjustSize();
}
