#include "infowidget.h"
#include "ui_infowidget.h"
#include "models/order/workmodel.h"
#include "entity/order/order.h"
#include "widgets/entitytable.h"
#include "utility/process.h"
#include <QDateTime>
#include "models/modelmanager.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    ui->invoiceCheck->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->releaseCheck->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->invoiceCheck->setFocusPolicy(Qt::NoFocus);
    ui->releaseCheck->setFocusPolicy(Qt::NoFocus);

    EntityTable::setup(ui->table);
    html.setFileName(":img/info.html");
    html.open(QFile::ReadOnly);
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::setOrder(const QModelIndex &index)
{
    ui->processes->clear();
    QTableView *table = (QTableView *) sender();
    QAbstractTableModel *model = (QAbstractTableModel *)table->model();
    Entity *orderView = (Entity *)model->data(index, PPLib::entityRole).toInt();
    if (orderView == NULL) {
        return;
    }
    int orderId = orderView->fields["ordr_id"].toInt();
    ui->header->setText(QString::fromUtf8("Заказ №%1").arg(orderId));

    Order *order = new Order();
    order->retrive(orderId, false);

    int status = order->fields["ordr_status"].toInt();
    ui->status->setText(Order::statuses[status]);

    setupInfo(order);
    setupFlow(order, orderView);

    int managerId = order->fields["client_id"].toInt();
    if (!Client::isOwner(managerId)) {
        ui->paymentFrame->hide();
        return;
    }

    setupFin(order, orderView);
}

void InfoWidget::setupInfo(Entity *order)
{
    html.seek(0);
    QString delivery;

    int deliveryType = order->fields["ordr_delivery"].toInt();
    if (deliveryType == Order::NEWPOST) {
        QString decl = order->fields["ordr_declaration"].toString();
        delivery.append(QString::fromUtf8("Декларация: %1. ").arg(decl));
    }
    delivery.append(order->fields["ordr_dinfo"].toString());

    int packType = order->fields["ordr_pack"].toInt();

    QString infoText = QString::fromUtf8(html.readAll())
            .arg(Order::deliveries[deliveryType])
            .arg(delivery)
            .arg(Order::packs[packType])
            .arg(order->fields["ordr_pinfo"].toString())
            .arg(order->fields["ordr_mark"].toString())
            .arg(order->fields["ordr_story"].toString());

    ui->info->setHtml(infoText);
}

void InfoWidget::setupFlow(Entity *order, Entity *orderView)
{
    int orderId = orderView->fields["ordr_id"].toInt();
    WorkModel *workModel = new WorkModel(this);
    workModel->setMode(WorkModel::PROCESS);
    QSqlQuery &query = workModel->getRefreshQuery();
    query.prepare(Work::allQuery);
    query.bindValue(":id", orderId);
    workModel->refresh();

    ui->table->setModel(workModel);
    ui->processInfo->setPlainText(order->fields["ordr_info"].toString());
    ui->dateProcessStart->setText(order->fields["ordr_startp"].
            toDateTime().toString(PPLib::dateTimeTemplate));
    ui->dateProcessEnd->setText(order->fields["ordr_endp"].
            toDateTime().toString(PPLib::dateTimeTemplate));

    int clientId = order->fields["client_start"].toInt();
    ClientModel *clientM = ModelManager::getInstance()->getClientModel();
    ui->whoStart->setText(clientM->getName(clientId).toString());

    clientId = order->fields["client_start"].toInt();
    ui->whoEnd->setText(clientM->getName(clientId).toString());

    ui->dateRelease->setText(order->fields["ordr_release"].
            toDateTime().toString(PPLib::dateTimeTemplate));

    clientId = order->fields["client_release"].toInt();
    ui->whoRelease->setText(clientM->getName(clientId).toString());
}

void InfoWidget::setupFin(Entity *order, Entity *orderView)
{
    Money total(order->fields["ordr_total"]);
    Money debt(order->fields["ordr_debt"]);
    ui->total->setText(total.toString());
    ui->pay->setText((total - debt).toString());
    ui->debt->setText(debt.toString());
    ui->invoiceCheck->setChecked(order->fields["ordr_isinvoice"].toBool());
    ui->releaseCheck->setChecked(order->fields["ordr_isrelease"].toBool());
}

void InfoWidget::on_table_clicked(const QModelIndex &index)
{
    QAbstractTableModel *model = (QAbstractTableModel *)ui->table->model();
    Entity *work = (Entity *)model->data(index, PPLib::entityRole).toInt();
    if (work == NULL) {
        return;
    }
    int tasksBits = work->fields["work_processes"].toInt();
    Process *process = Process::getInstance();
    ui->processes->setPlainText(process->getProcessStr(tasksBits));
}
