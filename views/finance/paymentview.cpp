#include "paymentview.h"
#include "ui_paymentview.h"
#include "widgets/entitytable.h"
#include "entity/order/order.h"
#include "entity/stock/material.h"
#include "dialogs/finance/paymentdialog.h"
#include "models/modelmanager.h"
#include "mainwindow.h"
#include <QDateTime>

PaymentView::PaymentView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentView)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Платежи"));

    ui->table->setContextMenuPolicy(Qt::CustomContextMenu);
    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");

    model = new PaymentModel(this);
    ui->table->setModel(model);

    QDate current = QDate::currentDate();
    ui->dateTill->blockSignals(true);
    ui->dateTill->setDate(current);
    ui->dateFrom->setDate(current.addMonths(-1));
    ui->dateTill->blockSignals(false);


    accountModel = ModelManager::getInstance()->getAccountModel();
    connect(accountModel, SIGNAL(modelReset()), this, SLOT(cacheChanged()));
    cacheChanged();

    appendMenu();

}

PaymentView::~PaymentView()
{
    delete ui;
}

void PaymentView::appendMenu()
{
    QAction *actionDepositReturn, *actionRefresh, *actionPayment;
    menu = new QMenu();

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), model, SLOT(refresh()));
    connect(actionRefresh, SIGNAL(triggered()), accountModel, SLOT(refresh()));

    actionPayment = new QAction(QString::fromUtf8("Добавить платеж"), this);
    connect(actionPayment, SIGNAL(triggered()), this, SLOT(addPayment()));

    actionDepositReturn = new QAction(QString::fromUtf8("Закрыть платеж"), this);
    connect(actionDepositReturn, SIGNAL(triggered()), this, SLOT(depositClose()));

    menu->addAction(actionPayment);
    menu->addAction(actionRefresh);
    menu->addSeparator();
    menu->addAction(actionDepositReturn);
}

void PaymentView::on_table_customContextMenuRequested(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}

void PaymentView::on_dateFrom_dateChanged(const QDate &date)
{
    refresh(date, ui->dateTill->date());
}

void PaymentView::on_dateTill_dateChanged(const QDate &date)
{
    refresh(ui->dateFrom->date(), date);
}

void PaymentView::addPayment()
{
    PaymentDialog *dialog = new PaymentDialog(this);
    dialog->setModel(model, PaymentDialog::OTHER);
    dialog->exec();
    delete dialog;
}

void PaymentView::cacheChanged()
{
    QModelIndex index = accountModel->index(Account::MAIN, 0);
    Account *main = (Account *) accountModel->data(index, PPLib::entityRole).toInt();
    ui->cache->setText(Money(main->fields["account_cache"]).toString());
    ui->nonCache->setText(Money(main->fields["account_noncache"]).toString());

    index = accountModel->index(Account::DEPOSIT, 0);
    Account *depo = (Account *) accountModel->data(index, PPLib::entityRole).toInt();
    ui->depoCache->setText(Money(depo->fields["account_cache"]).toString());
    ui->depoNonCache->setText(Money(depo->fields["account_noncache"]).toString());
}

void PaymentView::depositClose()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }
    Payment *payment = (Payment *)model->data(ui->table->currentIndex(),
                                         PPLib::entityRole).toInt();
    int debt = payment->getPayDebt();

    if (debt!= Pay::CREATEDEBT) {
        QMessageBox::critical(this, QString::fromUtf8("Ошибка"),
                              QString::fromUtf8("Этот платеж не требует закрытия"));

        return;
    }

    PaymentDialog *dialog = new PaymentDialog(this);
    dialog->setModel(model, PaymentDialog::DEPOSIT, payment);
    dialog->exec();
    delete dialog;
}

void PaymentView::refresh(QDate from, QDate till)
{
    QSqlQuery &query = model->getRefreshQuery();

    query.prepare(Payment::allByDate);
    query.bindValue(":from", from);
    query.bindValue(":till", till);
    query.bindValue(":client", Client::currentRole);
    model->refresh();
}
