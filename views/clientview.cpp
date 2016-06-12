#include "clientview.h"
#include "ui_clientview.h"

#include <QSortFilterProxyModel>
#include "models/clientmodel.h"
#include "entity/client.h"
#include "dialogs/clientdialog.h"
#include "models/modelmanager.h"

ClientView::ClientView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientView)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Пользователи"));
    ModelManager *models = ModelManager::getInstance();

    QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(this);
    sortModel->setSourceModel(models->getClientModel());

    ui->table->setModel(sortModel);

    connect(ui->table->actionAdd, SIGNAL(triggered()), this, SLOT(addClient()));
    connect(ui->table->actionEdit, SIGNAL(triggered()), this, SLOT(editClient()));
    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editClient(QModelIndex)));

}

ClientView::~ClientView()
{
    delete ui;
}

void ClientView::addClient()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    ClientDialog *dialog = new ClientDialog(this);
    if (!select->hasSelection()){
        dialog->set(ui->table->model(), PPLib::addRole, QModelIndex());
    } else {
        dialog->set(ui->table->model(), PPLib::addRole, select->currentIndex());
    }

    dialog->exec();
    delete dialog;
}

void ClientView::editClient()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editClient(select->currentIndex());
}

void ClientView::editClient(QModelIndex index)
{
    ClientDialog *dialog = new ClientDialog(this);
    dialog->set(ui->table->model(), Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}
