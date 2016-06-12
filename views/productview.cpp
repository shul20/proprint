#include "productview.h"
#include "ui_productview.h"
#include "models/productmodel.h"
#include "dialogs/productdialog.h"
#include "entity/stock/stock.h"
#include <QDebug>
#include "models/modelmanager.h"

ProductView::ProductView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductView)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("Продукция"));

    ModelManager *models = ModelManager::getInstance();

    stockF = new SortFilterModel(this);
    stockF->setSourceModel(models->getProductModel());
    ui->table->setModel(stockF);

    ui->stockType->addItem(QString::fromUtf8("Все"));
    for (int i = 0; i < Stock::stocks.length(); i++){
        ui->stockType->addItem(Stock::stocks[i]);
    }

    connect(ui->table->actionAdd, SIGNAL(triggered()), this, SLOT(addProduct()));
    connect(ui->table->actionEdit, SIGNAL(triggered()), this, SLOT(editProduct()));
    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editProduct(QModelIndex)));

}

ProductView::~ProductView()
{
    delete ui;
}

void ProductView::on_stockType_currentIndexChanged(int index)
{
    index -= 1;
    if (index == -1) {
        stockF->setFilterRole(-1);
    } else {
        stockF->setFilterRole(ProductModel::stockRole);
    }

    stockF->setMatchValue(index);

}

void ProductView::addProduct()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    ProductDialog *dialog = new ProductDialog(this);
    if (!select->hasSelection()){
        dialog->set(stockF, PPLib::addRole, QModelIndex());
    } else {
        dialog->set(stockF, PPLib::addRole, select->currentIndex());
    }

    dialog->exec();
    delete dialog;
}

void ProductView::editProduct()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editProduct(select->currentIndex());
}

void ProductView::editProduct(QModelIndex index)
{
    ProductDialog *dialog = new ProductDialog(this);
    dialog->set(stockF, Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}
