#include "orderprocessd.h"
#include "ui_orderprocessd.h"
#include <QDate>
#include "utility/process.h"
#include "widgets/workflow/printflow.h"
#include "mainwindow.h"

OrderProcessD::OrderProcessD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderProcessD), flowWidget(NULL)
{
    ui->setupUi(this);
    ui->checkFrame->setDisabled(true);
}

OrderProcessD::~OrderProcessD()
{
    delete ui;
    delete order;
}

void OrderProcessD::setModel(QAbstractItemModel *model, Entity *view)
{
    this->model = model;
    orderView = view;

    order = new Order();
    order->retrive(orderView->fields["ordr_id"].toInt(), false);

    setInfo();
    setFlowWidget(order);
    ui->order2->setOrder(order, Order2Widget::PROCESS);


    int status = order->fields["ordr_status"].toInt();
    if (status == Order::ARCHIVE) {
        flowWidget->setReadOnly(true);
        ui->saveButton->hide();
        ui->order2->setReadOnlyAll();
        return;
    }

    ui->order2->setReadOnly();

    if (status == Order::TOWORK) {
        flowWidget->setReadOnly(true);
        ui->saveButton->setText(QString::fromUtf8("Начать"));
        connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(startOrder()));
        return;
    }

    int managerId = order->fields["client_id"].toInt();
    int operatorId = order->fields["client_operate"].toInt();

    if (Client::isOwner(operatorId) || Client::isOwner(managerId)) {
        ui->saveButton->setText(QString::fromUtf8("Изменить"));
        connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(update()));

        if (status == Order::READY &&
                (Client::currentRole == Client::MANAGER ||
                 Client::currentRole == Client::ADMIN)) {

            ui->checkInvoice->setChecked(order->fields["ordr_isinvoice"].toBool());
            ui->checkRelease->setChecked(order->fields["ordr_isrelease"].toBool());
            ui->checkFrame->setEnabled(true);
        }
        return;
    }

    //ui->saveButton->hide();
    //flowWidget->setReadOnly(true);
}

void OrderProcessD::startOrder()
{
    MainWindow::db->transaction();
    if (!order->retrive(order->getId(), true) ||
         order->fields["ordr_status"].toInt() != Order::TOWORK) {
         MainWindow::db->rollback();
         QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
            QString::fromUtf8("Заказ заблокирован или уже начат"), QMessageBox::Ok);
        return;
    }

    order->fields["ordr_status"] = Order::PAUSE;
    if (!ui->order2->form2Entity(order) ||
        !order->update()){

        MainWindow::db->rollback();
    }
    orderView->fields["ordr_status"] = Order::PAUSE;

    MainWindow::db->commit();

    ui->saveButton->setText(QString::fromUtf8("Изменить"));
    disconnect(ui->saveButton, SIGNAL(clicked()), this, SLOT(startOrder()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(update()));

    flowWidget->setReadOnly(false);
}

void OrderProcessD::endOrder()
{
}


void OrderProcessD::update()
{    
    QString message;

    order->fields["ordr_isinvoice"] = ui->checkInvoice->isChecked();
    order->fields["ordr_isrelease"] = ui->checkRelease->isChecked();

    MainWindow::db->transaction();
    if (!flowWidget->update(order, message) ||
        !ui->order2->form2Entity(order) ||
        !order->update()) {

        if (message.length() == 0) {
            message = Order::lastError;
        }
        MainWindow::db->rollback();
        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), message);
        return;
    }
    MainWindow::db->commit();

    int status = order->fields["ordr_status"].toInt();
    if (status == Order::READY &&
        (Client::currentRole == Client::MANAGER ||
         Client::currentRole == Client::ADMIN)) {

        ui->checkFrame->setEnabled(true);
    }
}

void OrderProcessD::on_exitButton_clicked()
{
    close();
}

void OrderProcessD::setInfo()
{
    ui->company->setText(orderView->fields["company_name"].toString());
    ui->contact->setText(orderView->fields["contact_name"].toString());
    ui->dateStart->setText(order->fields["ordr_start"].toDateTime().toString(PPLib::dateTimeTemplate));
    ui->dateEnd->setText(order->fields["ordr_end"].toDateTime().toString(PPLib::dateTimeTemplate));
    setWindowTitle(QString::fromUtf8("Технологическая карта заказа №%1")
                       .arg(order->getId()));
}

void OrderProcessD::setFlowWidget(Entity *order)
{
    if (flowWidget != NULL) {
        ui->flowBox->layout()->removeWidget(flowWidget);
        delete flowWidget;
        flowWidget = NULL;
    }

    int processId = order->fields["ordr_process"].toInt();

    if (processId == Process::PRINT){
        flowWidget = new PrintFlow(this);
        ui->flowBox->layout()->addWidget(flowWidget);
        flowWidget->setOrder(order);
    }
}
