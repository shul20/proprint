#include "invoicedialog.h"
#include "ui_invoicedialog.h"
#include <QFile>
#include <QDebug>
#include <QPrintDialog>
#include <QPrinter>
#include <QDateTime>
#include "models/order/workmodel.h"
#include "entity/client.h"

InvoiceDialog::InvoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceDialog)
{
    ui->setupUi(this);
}

InvoiceDialog::~InvoiceDialog()
{
    delete ui;
}

void InvoiceDialog::setOrder(Entity *order, Entity *view)
{
    int orderId = order->getId();

    WorkModel *workModel = new WorkModel(this);
    workModel->setMode(WorkModel::PROCESS);
    QSqlQuery &query = workModel->getRefreshQuery();
    query.prepare(Work::allQuery);
    query.bindValue(":id", orderId);
    workModel->refresh();

    QString works, productName, stockName;
    productName = view->fields["product_name"].toString();
    stockName = view->fields["stock_name"].toString();

    for(int row = 0; row < workModel->rowCount(); row++) {
        QModelIndex index = workModel->index(row, 0);
        Work *work = (Work *) workModel->data(index, PPLib::entityRole).toInt();
        QString workRow = QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>")
                .arg(productName)
                .arg(stockName)
                .arg(work->fields["work_width"].toInt())
                .arg(work->fields["work_height"].toInt())
                .arg(work->fields["work_amount"].toInt());

        works.append(workRow);
    }

    QFile html(":img/invoice.html");
    html.open(QFile::ReadOnly);
    QString text = QString::fromUtf8(html.readAll())
            .arg(order->getId())
            .arg(view->fields["company_name"].toString())
            .arg(view->fields["contact_name"].toString())
            .arg(view->fields["ordr_start"].toDateTime().toString(PPLib::dateTimeTemplate))
            .arg(works)
            .arg(Client::currentClient->fields["client_name"].toString())
            .arg(QDateTime::currentDateTime().toString(PPLib::dateTimeTemplate));

    ui->textEdit->setHtml(text);
    ui->textEdit->append(text);
}

void InvoiceDialog::on_exitButton_clicked()
{
    close();
}

void InvoiceDialog::on_printButton_clicked()
{
    QPrinter printer;
    printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);
     QPrintDialog *dialog = new QPrintDialog(&printer, this);
     dialog->setWindowTitle(tr("Print Document"));
     if (dialog->exec() == QDialog::Accepted) {
         ui->textEdit->print(&printer);
     }
}
