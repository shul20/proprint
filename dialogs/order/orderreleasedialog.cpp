#include "orderreleasedialog.h"
#include "ui_orderreleasedialog.h"
#include <QDateTime>
#include "entity/client.h"
#include "entity/order/order.h"
#include "dialogs/order/invoicedialog.h"
#include "mainwindow.h"
#include "entity/company.h"
#include "models/modelmanager.h"

OrderReleaseDialog::OrderReleaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderReleaseDialog), order(NULL)
{
    ui->setupUi(this);
}

OrderReleaseDialog::~OrderReleaseDialog()
{
    delete ui;
    delete order;
}

void OrderReleaseDialog::setModel(QAbstractItemModel *, Entity *view)
{
    this->view = view;

    QDateTime date = QDateTime::currentDateTime();

    ui->date->setText(date.toString(PPLib::dateTimeTemplate));
    ui->client->setText(Client::currentClient->fields["client_name"].toString());

    int orderId = view->fields["ordr_id"].toInt();
    setWindowTitle(QString::fromUtf8("Заказ №%1").arg(orderId));
}

void OrderReleaseDialog::on_no_clicked()
{
    close();
}

void OrderReleaseDialog::on_invoice_clicked()
{
    InvoiceDialog *dialog = new InvoiceDialog(this);
    dialog->setOrder(order, view);
    dialog->exec();
    delete dialog;
}

void OrderReleaseDialog::on_yes_clicked()
{
    int orderId = view->fields["ordr_id"].toInt();
    order = new Order();

    MainWindow::db->transaction();
    if (!order->retrive(orderId, true)) {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
                       QString::fromUtf8("Заказ заблокирован. \n Попробуйте позже"),
                       QMessageBox::Ok);

        MainWindow::db->rollback();
        return;
    }

    if (!order->fields["ordr_isrelease"].toBool()) {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
                       QString::fromUtf8("Заказ не разрешен к выдаче"),
                       QMessageBox::Ok);

        MainWindow::db->rollback();
        return;
    }

    int status = order->fields["ordr_status"].toInt();
    if ( status != Order::READY) {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
                              QString::fromUtf8("Статус заказа: %1").
                              arg(Order::statuses[status]), QMessageBox::Ok);

        MainWindow::db->rollback();
        return;
    }

    order->fields["ordr_release"] = QDateTime::currentDateTime();
    order->fields["client_release"] = Client::currentClient->getId();
    order->fields["ordr_status"] = Order::ARCHIVE;

    int companyId = view->fields["company_id"].toInt();
    int contactId = view->fields["contact_id"].toInt();
    int total = order->fields["ordr_total"].toInt();

    if (!order->update() ||
        !Company::updateDebt(companyId, total) ||
        !Contact::updateDebt(contactId, total)) {
        MainWindow::db->rollback();
        return;
    }
    MainWindow::db->commit();
    ui->invoice->setEnabled(true);
    ui->no->setText(QString::fromUtf8("Выход"));
}
