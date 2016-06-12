#include "orderstart.h"
#include "ui_orderstart.h"
#include <QDebug>
#include "widgets/entitytable.h"
#include "dialogs/order/orderdialog.h"
#include "dialogs/order/orderprocessd.h"
#include "models/order/orderstartm.h"
#include "entity/client.h"
#include "models/varmodel.h"
#include "models/modelmanager.h"
#include "dialogs/finance/paymentdialog.h"

OrderStart::OrderStart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderStart)
{
    ui->setupUi(this);
    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    setPermission();
}

OrderStart::~OrderStart()
{
    delete ui;
}

void OrderStart::setup(WorkFlow *workFlow, InfoWidget *info)
{
    this->workFlow = workFlow;

    model = new OrderStartM(this);
    QString queryStr = QString(Order::viewByStatus).arg(allowStatuses);

    QSqlQuery &query = model->getRefreshQuery();
    query.prepare(queryStr);
    model->refresh();

    ui->table->setModel(model);


    connect(ui->table, SIGNAL(clicked(QModelIndex)), info,
            SLOT(setOrder(QModelIndex)));
    appendMenu();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
}

void OrderStart::appendMenu()
{
    QAction *actionAdd, *actionEdit, *actionRefresh, *actionDel, *actionPayment;
    menu = new QMenu();

    actionAdd = new QAction(QString::fromUtf8("Добавить"), this);
    connect(actionAdd, SIGNAL(triggered()), this, SLOT(addOrder()));

    actionEdit = new QAction(QString::fromUtf8("Изменить"), this);
    connect(actionEdit, SIGNAL(triggered()), this, SLOT(editOrder()));

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), model, SLOT(refresh()));

    actionDel = new QAction(QString::fromUtf8("Удалить"), this);
    connect(actionDel, SIGNAL(triggered()), this, SLOT(delOrder()));


    QMenu *menuStatus = new QMenu(QString::fromUtf8("Установить статус"));
    for (int i = Order::PREPARE; i < Order::PROCESS; i++) {
        QAction *action = new QAction(Order::statuses[i], this);
        action->setData(i);
        menuStatus->addAction(action);
     }
    connect(menuStatus, SIGNAL(triggered(QAction*)), this, SLOT(setStatus(QAction*)));

    QMenu *menuPriority = new QMenu(QString::fromUtf8("Задать приоритет"));
    for (int priority = 0; priority < 6; priority++) {
        QAction *action = new QAction(QString("%1").arg(priority), this);
        action->setData(priority);
        menuPriority->addAction(action);
     }
    connect(menuPriority, SIGNAL(triggered(QAction*)), this, SLOT(setPriority(QAction*)));

    actionPayment = new QAction(QString::fromUtf8("Внести проплату"), this);
    connect(actionPayment, SIGNAL(triggered()), this, SLOT(paymentOrder()));

    if ((Client::currentRole == Client::ADMIN) ||
        (Client::currentRole == Client::MANAGER)) {

        menu->addAction(actionAdd);
        menu->addAction(actionEdit);
        menu->addAction(actionRefresh);
        menu->addAction(actionDel);
        menu->addSeparator();
        menu->addMenu(menuStatus);
        menu->addMenu(menuPriority);
        menu->addSeparator();
        menu->addAction(actionPayment);

    } else {
        menu->addAction(actionEdit);
        menu->addAction(actionRefresh);
    }

    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(modifyOrder(QModelIndex)));

}

void OrderStart::setPermission()
{
    if ((Client::currentRole == Client::ADMIN) ||
        (Client::currentRole == Client::MANAGER)) {
        allowStatuses = QString("%1, %2, %3")
                 .arg(Order::PREPARE).arg(Order::TOWORK)
                 .arg(Order::PAUSE);

    } else if (Client::currentRole == Client::PRINT){
        allowStatuses = QString("%1, %2")
                 .arg(Order::TOWORK)
                 .arg(Order::PAUSE);

    } else if (Client::currentRole == Client::POSTPRINT){
        allowStatuses = QString("%1, %2")
                 .arg(Order::TOWORK)
                 .arg(Order::PAUSE);
    }
}

void OrderStart::setStatus(QAction* action)
{
    Entity *order = currentEntity();

    if (order == NULL ||
        !Client::isOwner(order->fields["client_id"].toInt())){

        PPLib::accessDeniedMessage();
        return;
    }
    int orderId = order->fields["ordr_id"].toInt();
    int status = action->data().toInt();
    if (Order::setStatus(orderId, status)) {
        order->fields["ordr_status"] = status;
    }
}

void OrderStart::setPriority(QAction* action)
{
    Entity *order = currentEntity();

    if (order == NULL ||
        !Client::isOwner(order->fields["client_id"].toInt())){
        PPLib::accessDeniedMessage();
        return;
    }

    int orderId = order->fields["ordr_id"].toInt();
    int priority = action->data().toInt();

    if (Order::setPriority(orderId, priority)) {
        order->fields["ordr_priority"] = priority;
    }
}

void OrderStart::addOrder()
{    
    OrderDialog *dialog = new OrderDialog(this);
    Entity *orderView = currentEntity();

    if (orderView != NULL &&
        Client::isOwner(orderView->fields["client_id"].toInt())){
        dialog->setModel(model,  Order::ADD, orderView);

    } else {
        dialog->setModel(model, Order::ADD, NULL);
    }

    dialog->exec();
    delete dialog;
}

void OrderStart::delOrder()
{
    if (PPLib::delRowMessage() == QMessageBox::Cancel){
        return;
    }

    Entity *orderView = currentEntity();

    if (orderView == NULL ||
        !Client::isOwner(orderView->fields["client_id"].toInt())){

        PPLib::accessDeniedMessage();
        return;
    }

    int orderId = orderView->fields["ordr_id"].toInt();
    Order *order = new Order();
    order->fields["ordr_id"] = orderId;

    if (!order->del()){

        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), Order::lastError);
        delete order;
        return;
    }

    model->removeRow(ui->table->currentIndex().row());
    delete order;
}

void OrderStart::editOrder()
{
    modifyOrder(ui->table->currentIndex());
}

void OrderStart::modifyOrder(QModelIndex index)
{
    Entity *orderView = currentEntity();

    if (orderView == NULL) {
        return;
    }

    int managerId = orderView->fields["client_id"].toInt();
    int status = orderView->fields["ordr_status"].toInt();

    if (status == Order::PREPARE) {
        if (!Client::isOwner(managerId)){

            PPLib::accessDeniedMessage();
            return;
        }
        OrderDialog *dialog = new OrderDialog(this);
        dialog->setModel(model, Order::EDIT, orderView);
        dialog->exec();
        delete dialog;

    } else if (status == Order::TOWORK || status == Order::PAUSE) {
        int operatorId = orderView->fields["client_operate"].toInt();

        if (Client::isOwner(managerId) ||
            Client::isOwner(operatorId)){

            OrderProcessD *dialog = new OrderProcessD(this);
            dialog->setModel(model, orderView);
            dialog->exec();
            delete dialog;
            return;
        }

        PPLib::accessDeniedMessage();
        return;
    }
}

void OrderStart::paymentOrder()
{
    Entity * orderView = currentEntity();
    if (orderView == NULL) {
        return;
    }

    PaymentDialog *dialog = new PaymentDialog(this);
    dialog->setModel(NULL, PaymentDialog::ORDER, orderView);
    dialog->exec();
    delete dialog;
}

Entity *OrderStart::currentEntity()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return NULL;
    }
    return (Entity *)model->data(ui->table->currentIndex(),
                                 PPLib::entityRole).toInt();
}

void OrderStart::stopTimer()
{
    timer->stop();
}


void OrderStart::on_table_customContextMenuRequested(const QPoint &pos)
{
    timer->stop();
    menu->exec(QCursor().pos());
    timer->start(timer->interval());
}

void OrderStart::refresh()
{
    VarModel *varModel = ModelManager::getInstance()->getVarModel();
    timer->setInterval(varModel->getVar(Var::OREFRESH).toInt());
    model->refresh();
}

bool OrderStart::event(QEvent *event)
{
    if (event->type() == QEvent::Show){
        VarModel *varModel = ModelManager::getInstance()->getVarModel();
        timer->start(varModel->getVar(Var::OREFRESH).toInt());

    } else if (event->type() == QEvent::Hide) {
        timer->stop();
    }

    return true;
}
