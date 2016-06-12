#include "priceview.h"
#include "ui_priceview.h"
#include "QSortFilterProxyModel"
#include "models/stock/pricemodel.h"
#include "models/processmodel.h"

PriceView::PriceView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PriceView), findF(NULL)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Прайсы"));


    PriceModel *priceModel = new PriceModel(this);
    QSqlQuery &query = priceModel->getRefreshQuery();
    query.prepare(Price::allQuery);
    priceModel->refresh();

    priceF = new SortFilterModel(this);
    priceF->setSourceModel(priceModel);
    priceF->setFilterRole(PriceModel::processRole);

    ProcessModel *processModel = new ProcessModel(this, Process::ISPRICE);
    ui->process->setModel(processModel);

    ui->table->setModel(priceF);

    connect(ui->table->actionAdd, SIGNAL(triggered()), this, SLOT(addPrice()));
    connect(ui->table->actionEdit, SIGNAL(triggered()), this, SLOT(editPrice()));
    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editPrice(QModelIndex)));
}

PriceView::~PriceView()
{
    delete ui;
}

void PriceView::addPrice()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    PriceDialog *dialog = new PriceDialog(this);
    if (!select->hasSelection()){
        dialog->set(ui->table->model(), PPLib::addRole, QModelIndex());
    } else {
        dialog->set(ui->table->model(), PPLib::addRole, select->currentIndex());
    }

    dialog->exec();
    delete dialog;
}

void PriceView::editPrice()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editPrice(select->currentIndex());
}

void PriceView::editPrice(QModelIndex index)
{
    PriceDialog *dialog = new PriceDialog(this);
    dialog->set(ui->table->model(), Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}

void PriceView::on_process_currentIndexChanged(int index)
{
    int processId = ui->process->itemData(index, PPLib::idRole).toInt();
    priceF->setMatchValue(processId);
}

void PriceView::on_company_textChanged(const QString &arg1)
{
    if (arg1.length() < 3) {
        if (ui->table->model() != priceF) {
            ui->table->setModel(priceF);
        }
        return;
    }

    if (findF == NULL) {
        findF = new SortFilterModel(this);
        findF->setSourceModel(priceF);
        findF->setFilterRole(PriceModel::companyRole);
        findF->compare = &SortFilterModel::contain;
    }
    if (ui->table->model() != findF) {
        ui->table->setModel(findF);
    }

    findF->setMatchValue(arg1);
}
