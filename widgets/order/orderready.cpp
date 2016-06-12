#include "orderready.h"
#include "ui_orderready.h"
#include "widgets/entitytable.h"
#include "entity/order/order.h"
#include "models/varmodel.h"
#include "models/modelmanager.h"
#include "dialogs/order/orderprocessd.h"
#include "dialogs/order/orderdialog.h"
#include "dialogs/order/orderreleasedialog.h"

OrderReady::OrderReady(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderReady)
{
    ui->setupUi(this);

    EntityTable::setup(ui->table);
}

OrderReady::~OrderReady()
{
    delete ui;
}

void OrderReady::setup(InfoWidget *info)
{
    model = new OrderReadyM(this);
    QString queryStr = QString(Order::viewByStatus).arg(Order::READY);

    QSqlQuery &query = model->getRefreshQuery();
    query.prepare(queryStr);
    model->refresh();

    ui->table->setModel(model);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));

    appendMenu();

    connect(ui->table, SIGNAL(clicked(QModelIndex)), info,
            SLOT(setOrder(QModelIndex)));

}

void OrderReady::hide()
{
    timer->stop();
    QWidget::hide();
}

void OrderReady::show()
{
    QWidget::show();

    if (!timer->isActive()){
        timer->start(timer->interval());
    }
}

void OrderReady::on_table_customContextMenuRequested(const QPoint &pos)
{
    timer->stop();
    menu->exec(QCursor().pos());
    timer->start(timer->interval());
}

void OrderReady::openOrder()
{
    Entity * orderView = currentEntity();
    if (orderView == NULL) {
        return;
    }

    Order::openOrder(model, Order::VIEW, orderView);
}

void OrderReady::openProcess()
{
    Entity * orderView = currentEntity();
    if (orderView == NULL) {
        return;
    }

    Order::openProcess(model, orderView);
}

void OrderReady::releaseOrder()
{
    Entity *orderView = currentEntity();

    if (orderView == NULL) {
        return;
    }
    int managerId = orderView->fields["client_id"].toInt();
    int operatorId = orderView->fields["client_operate"].toInt();

    if (Client::isOwner(managerId) ||  Client::isOwner(operatorId)){

        OrderReleaseDialog *dialog = new OrderReleaseDialog(this);
        dialog->setModel(model, orderView);
        dialog->exec();
        delete dialog;
        return;
    }

    PPLib::accessDeniedMessage();
}

void OrderReady::pauseOrder()
{
    Entity *order = currentEntity();

    if (order == NULL ||
        !Client::isOwner(order->fields["client_id"].toInt())){

        PPLib::accessDeniedMessage();
        return;
    }
    int orderId = order->fields["ordr_id"].toInt();
    if (Order::setStatus(orderId, Order::PAUSE)) {
        order->fields["ordr_status"] = Order::PAUSE;
    }
}

void OrderReady::modifyOrder(QModelIndex)
{
}

void OrderReady::appendMenu()
{
    QAction *actionOrder, *actionProcess, *actionRefresh,
            *actionRelease, *actionPause;
    menu = new QMenu();

    actionOrder = new QAction(QString::fromUtf8("Открыть заказ"), this);
    connect(actionOrder, SIGNAL(triggered()), this, SLOT(openOrder()));

    actionProcess = new QAction(QString::fromUtf8("Открыть процесс"), this);
    connect(actionProcess, SIGNAL(triggered()), this, SLOT(openProcess()));

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), model, SLOT(refresh()));

    actionRelease = new QAction(QString::fromUtf8("Выдать"), this);
    connect(actionRelease, SIGNAL(triggered()), this, SLOT(releaseOrder()));

    actionPause = new QAction(QString::fromUtf8("Пауза"), this);
    connect(actionPause, SIGNAL(triggered()), this, SLOT(pauseOrder()));


    menu->addAction(actionProcess);
    menu->addAction(actionRefresh);
    menu->addAction(actionRelease);

    if ((Client::currentRole == Client::ADMIN) ||
        (Client::currentRole == Client::MANAGER)) {
        menu->addAction(actionOrder);
        menu->addAction(actionPause);
    }

    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(modifyOrder(QModelIndex)));

}

Entity *OrderReady::currentEntity()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return NULL;
    }
    return (Entity *)model->data(ui->table->currentIndex(),
                                 PPLib::entityRole).toInt();
}

void OrderReady::refresh()
{
    VarModel *varModel = ModelManager::getInstance()->getVarModel();
    timer->setInterval(varModel->getVar(Var::OREFRESH).toInt());
    model->refresh();
}

bool OrderReady::event(QEvent *event)
{
    if (event->type() == QEvent::Show){
        VarModel *varModel = ModelManager::getInstance()->getVarModel();
        timer->start(varModel->getVar(Var::OREFRESH).toInt());

    } else if (event->type() == QEvent::Hide) {
        timer->stop();
    }

    return true;
}
