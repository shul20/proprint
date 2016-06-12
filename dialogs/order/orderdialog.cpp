#include "orderdialog.h"
#include "ui_orderdialog.h"
#include "models/sortfiltermodel.h"
#include "views/contactview.h"
#include "models/processmodel.h"
#include "entity/order/order.h"
#include "entity/client.h"
#include "entity/stock/stock.h"
#include "entity/equip/equipment.h"
#include "entity/entityview.h"
#include "mainwindow.h"
#include "widgets/work/printwidget.h"

OrderDialog::OrderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderDialog), workWidget(NULL),
    model(NULL), mode(Order::VIEW), orderView(NULL)
{
    ui->setupUi(this);
    ui->price->setup(false);
    ui->endDate->setCalendarPopup(true);

    ProcessModel *processModel = new ProcessModel(this, Process::ISORDER);
    ui->process->blockSignals(true);
    ui->process->setModel(processModel);
    ui->process->blockSignals(false);

    ui->company->setup(Company::CUSTOMER, true, true, true, true);
}

OrderDialog::~OrderDialog()
{
    delete ui;
    delete order;
}

void OrderDialog::setModel(QAbstractItemModel *model, int mode, Entity *view)
{
    this->model = model;
    this->mode = mode;

    if (view != NULL) {
        order = new Order();
        order->retrive(view->fields["ordr_id"].toInt(), false);

        entity2Form(order, view);
        int orderId = order->getId();

        if (mode == Order::ADD) {
            workWidget->setOrder(orderId, true);
            ui->order2->setOrder(order, Order2Widget::CREATE, true);
            order->fields.remove("ordr_id");

            ui->saveOrderButton->setText(QString::fromUtf8("Создать"));
            setWindowTitle(QString::fromUtf8("Создать заказ"));
            blockInput(true);

        } else if (mode == Order::EDIT) {
            workWidget->setOrder(orderId);
            ui->order2->setOrder(order, Order2Widget::CREATE, false);

            ui->saveOrderButton->setText(QString::fromUtf8("Изменить"));
            setWindowTitle(QString::fromUtf8("Редактирование заказа №%1").arg(orderId));
            blockInput(true);

        } else if (mode == Order::VIEW) {
            workWidget->setOrder(orderId);
            ui->order2->setOrder(order, Order2Widget::CREATE, false);
            ui->order2->setReadOnlyAll();

            ui->saveOrderButton->hide();
            setWindowTitle(QString::fromUtf8("Просмотр заказа №%1").arg(orderId));
            setReadOnly();
        }

    } else {
        order = new Order();

        clear();
        workWidget->clear();
        ui->order2->setOrder(order, Order2Widget::CREATE);

        ui->saveOrderButton->setText(QString::fromUtf8("Создать"));
        setWindowTitle(QString::fromUtf8("Создать заказ"));
    }

}


bool OrderDialog::form2Entity(Entity *entity)
{
     int index;
    index = ui->process->currentIndex();

    order->fields["ordr_process"] = ui->process->itemData(index, PPLib::idRole);
    order->fields["contact_id"] = ui->company->getContact()->getId();
    order->fields["equipment_id"] = workWidget->getEquip()->getId();

    ProductWidget *productW = workWidget->getProductW();
    Stock *stock = productW->getMaterial();
    if (stock == NULL) {
        return false;
    }

    order->fields["stock_id"] = stock->getId();
    order->fields["ordr_pricetype"] = ui->price->getPriceType()->getId();

    if (order->fields["client_id"].isNull()) {
        order->fields["client_id"] = Client::currentClient->getId();
    }

    order->fields["ordr_status"] = Order::PREPARE;

    QDateTime start;
    start.setDate(ui->startDate->date());
    start.setTime(ui->startTime->time());
    order->fields["ordr_start"] = start;

    QDateTime end;
    end.setDate(ui->endDate->date());
    end.setTime(ui->endTime->time());
    order->fields["ordr_end"] = end;

    order->fields["ordr_size"] = workWidget->getTotalSize().value;
    order->fields["ordr_total"] = workWidget->getTotal().cents;
    order->fields["ordr_debt"] = order->fields["ordr_total"];
    order->fields["ordr_totalprint"] = workWidget->getTotalPrint().cents;
    order->fields["ordr_priority"] = 0;
    order->fields["ordr_startp"] = QVariant();

    return true;
}

void OrderDialog::entity2Form(Entity *order, Entity *orderView)
{    
    ui->company->setContact(orderView->fields["company_id"].toInt(),
                            order->fields["contact_id"].toInt());

    int processId = order->fields["ordr_process"].toInt();
    int indx = ui->process->findData(processId, PPLib::idRole);
    ui->process->setCurrentIndex(-1);
    ui->process->setCurrentIndex(indx);

    ui->price->setType(order->fields["ordr_pricetype"].toInt());

    if (mode == Order::ADD) {
        ui->startDate->setDate(QDate::currentDate());
        ui->startTime->setTime(QTime::currentTime());
        ui->endDate->setDate(QDate::currentDate().addDays(3));
        ui->endTime->setTime(QTime(12, 0));

    } else {
        ui->startDate->setDate(order->fields["ordr_start"].toDate());
        ui->startTime->setTime(order->fields["ordr_start"].toTime());
        ui->endDate->setDate(order->fields["ordr_end"].toDate());
        ui->endTime->setTime(order->fields["ordr_end"].toTime());
    }
    workWidget->setTotalSize(Size(order->fields["ordr_size"]));
    workWidget->setTotal(Money(order->fields["ordr_total"]),
            Money(order->fields["ordr_totalprint"]));

}

void OrderDialog::on_saveOrderButton_clicked()
{
    if (!form2Entity(order) ||
        !ui->order2->form2Entity(order) ||
        !order->update() ||
        !workWidget->update(order->getId())) {

        //delete orderView;
        return;
    }

    QDialog::accept();
}

void OrderDialog::clear()
{
    ui->startDate->setDate(QDate::currentDate());
    ui->startTime->setTime(QTime::currentTime());
    ui->endDate->setCalendarPopup(true);
    ui->endDate->setDate(QDate::currentDate().addDays(3));
    ui->endTime->setTime(QTime(12, 0));
    ui->process->setCurrentIndex(-1);
    ui->process->setCurrentIndex(0);
}

void OrderDialog::on_process_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }
    if (workWidget != NULL) {
        ui->workBox->layout()->removeWidget(workWidget);
        delete workWidget;
        workWidget = NULL;
    }

    int processId = ui->process->itemData(index, PPLib::idRole).toInt();

    if (processId == Process::PRINT){
        workWidget = new PrintWidget(this);
        workWidget->setWidgets(ui->price, ui->company);
        ui->workBox->layout()->addWidget(workWidget);
        connect(workWidget, SIGNAL(worksChaged()), this, SLOT(worksChanged()));
        connect(workWidget, SIGNAL(blockInput(bool)),
                this, SLOT(blockInput(bool)));

    }
}

void OrderDialog::worksChanged()
{
   ui->totalSize->setText(workWidget->getTotalSize().toString());
   Money &total = workWidget->getTotal();
   Money &totalPrint = workWidget->getTotalPrint();
   ui->total->setMoney(total);
   ui->totalPrint->setMoney(totalPrint);
   ui->totalPostPrint->setMoney(total - totalPrint);
}

void OrderDialog::blockInput(bool check)
{
    ui->company->blockInput(check);
    ui->price->setDisabled(check);
    ui->process->setDisabled(check);
}

void OrderDialog::on_exitOrderButton_clicked()
{
    close();
}

void OrderDialog::setReadOnly()
{
    ui->infoBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->company->setReadOnly();
}
