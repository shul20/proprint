#include "orderarchw.h"
#include "ui_orderarchw.h"
#include "widgets/entitytable.h"
#include "entity/client.h"
#include "entity/order/order.h"
#include "dialogs/order/orderdialog.h"
#include "dialogs/finance/paymentdialog.h"

OrderArchW::OrderArchW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderArchW)
{
    ui->setupUi(this);

    ui->table->setContextMenuPolicy(Qt::CustomContextMenu);
    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
}

OrderArchW::~OrderArchW()
{
    delete ui;
}

void OrderArchW::setup(InfoWidget *info)
{
    model = new OrderArchM(this);
    ui->table->setModel(model);
    appendMenu();

    QDate current = QDate::currentDate();
    ui->dateTill->blockSignals(true);
    ui->dateTill->setDate(current);
    ui->dateFrom->setDate(current.addMonths(-1));
    ui->dateTill->blockSignals(false);

    connect(ui->table, SIGNAL(clicked(QModelIndex)), info,
            SLOT(setOrder(QModelIndex)));
}

void OrderArchW::appendMenu()
{
    QAction *actionOrder, *actionProcess, *actionRefresh,
            *actionRepeat, *actionPayment;
    menu = new QMenu();

    actionOrder = new QAction(QString::fromUtf8("Открыть заказ"), this);
    connect(actionOrder, SIGNAL(triggered()), this, SLOT(openOrder()));

    actionProcess = new QAction(QString::fromUtf8("Открыть процесс"), this);
    connect(actionProcess, SIGNAL(triggered()), this, SLOT(openProcess()));

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), model, SLOT(refresh()));

    actionRepeat = new QAction(QString::fromUtf8("Повторить"), this);
    connect(actionRepeat, SIGNAL(triggered()), this, SLOT(repeatOrder()));

    actionPayment = new QAction(QString::fromUtf8("Внести проплату"), this);
    connect(actionPayment, SIGNAL(triggered()), this, SLOT(paymentOrder()));


    menu->addAction(actionProcess);
    menu->addAction(actionRefresh);


    if ((Client::currentRole == Client::ADMIN) ||
        (Client::currentRole == Client::MANAGER)) {
        menu->addAction(actionOrder);
        menu->addAction(actionRepeat);
        menu->addSeparator();
        menu->addAction(actionPayment);
    }
}

Entity *OrderArchW::currentEntity()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return NULL;
    }
    return (Entity *)model->data(ui->table->currentIndex(),
                                 PPLib::entityRole).toInt();
}

void OrderArchW::refresh(QDate from, QDate till)
{
    QSqlQuery &query = model->getRefreshQuery();

    query.prepare(Order::viewArch);
    query.bindValue(":status", Order::ARCHIVE);
    query.bindValue(":from", from);
    query.bindValue(":till", till);
    model->refresh();
}

void OrderArchW::on_table_customContextMenuRequested(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}

void OrderArchW::on_dateFrom_dateChanged(const QDate &date)
{
    refresh(date, ui->dateTill->date());
}

void OrderArchW::on_dateTill_dateChanged(const QDate &date)
{
    refresh(ui->dateFrom->date(), date);
}

void OrderArchW::openOrder()
{
    Entity * orderView = currentEntity();
    if (orderView == NULL) {
        return;
    }

    Order::openOrder(model, Order::VIEW, orderView);
}

void OrderArchW::openProcess()
{
    Entity * orderView = currentEntity();
    if (orderView == NULL) {
        return;
    }

    Order::openProcess(model, orderView);
}

void OrderArchW::repeatOrder()
{    
    Entity * orderView = currentEntity();
    if (orderView == NULL) {
        return;
    }

    Order::openOrder(model, Order::ADD, orderView);
}

void OrderArchW::paymentOrder()
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

void OrderArchW::on_searchlButton_clicked()
{
    QString search = ui->searchEdit->text();

    if (search.length() == 0) {
        refresh(ui->dateFrom->date(), ui->dateTill->date());
        return;
    }

    search = QString::fromUtf8("\%%1\%").arg(search);

    QSqlQuery &query = model->getRefreshQuery();

    query.prepare(Order::searchStoryArch);
    query.bindValue(":status", Order::ARCHIVE);
    query.bindValue(":search", search);
    model->refresh();
}
