#include "payview.h"
#include "ui_payview.h"

#include "dialogs/finance/paydialog.h"
#include "models/finance/paymodel.h"
#include "entity/finance/pay.h"
#include "models/modelmanager.h"

PayView::PayView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayView)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Платежи"));
    ModelManager *models = ModelManager::getInstance();
    QSortFilterProxyModel *modelS = new QSortFilterProxyModel(this);
    modelS->setSourceModel(models->getPayModel());
    ui->table->setModel(modelS);

    connect(ui->table->actionAdd, SIGNAL(triggered()), this, SLOT(addPay()));
    connect(ui->table->actionEdit, SIGNAL(triggered()), this, SLOT(editPay()));
    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editPay(QModelIndex)));
}

PayView::~PayView()
{
    delete ui;
}

void PayView::addPay()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    PayDialog *dialog = new PayDialog(this);
    if (!select->hasSelection()){
        dialog->set(ui->table->model(), PPLib::addRole, QModelIndex());
    } else {
        dialog->set(ui->table->model(), PPLib::addRole, select->currentIndex());
    }

    dialog->exec();
    delete dialog;
}

void PayView::editPay()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editPay(select->currentIndex());
}

void PayView::editPay(QModelIndex index)
{
    PayDialog *dialog = new PayDialog(this);
    dialog->set(ui->table->model(), Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}
