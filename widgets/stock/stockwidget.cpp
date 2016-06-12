#include "stockwidget.h"
#include "ui_stockwidget.h"
#include <QDebug>
#include <QItemSelectionModel>
#include "entity/stock/material.h"
#include "dialogs/stock/materialdialog.h"
#include "dialogs/stock/cutrolldialog.h"
#include "dialogs/stock/stockdialog.h"
#include "dialogs/stock/woffdialog.h"
#include "views/stock/stockview.h"
#include "models/stock/stockproxy.h"

StockWidget::StockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockWidget), stockF(NULL)
{
    ui->setupUi(this);

    productW = new ProductWidget(this);
    ui->frameProduct->layout()->addWidget(productW);
    productW->setup(true, false, false, false, false);

    ui->frameProduct->setDisabled(true);

    connect(productW, SIGNAL(productChanged(Product*)),
            this, SLOT(productChanged(Product*)));

}

StockWidget::~StockWidget()
{
    delete ui;
}

void StockWidget::set(int type)
{
    stockType = type;

    StockProxy *proxy = new StockProxy(this, type);

    stockF = new SortFilterModel(this);
    stockF->setSourceModel(proxy);
    stockF->setFilterRole(-1);

    ui->table->setModel(stockF);
    ui->name->setText(proxy->getTitle());

    productW->setRole(ProductModel::stockRole, stockType);
    appendMenu();
}


void StockWidget::addStock()
{
    QItemSelectionModel *select = ui->table->selectionModel();
    QModelIndex index;

    if (select->hasSelection()){
        index = ui->table->currentIndex();
    }

    StockDialog *dialog = new StockDialog(this);
    dialog->set(ui->table->model(), PPLib::addRole, index, stockType);
    dialog->exec();
    delete dialog;
}

void StockWidget::editStock()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editStock(select->currentIndex());
}

void StockWidget::editStock(QModelIndex index)
{
    StockDialog *dialog = new StockDialog(this);
    dialog->set(ui->table->model(), Qt::EditRole, index, stockType);
    dialog->exec();
    delete dialog;
}

void StockWidget::addMaterial()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    QAbstractItemModel *materialModel = StockView::instance->getMaterialModel();
    MaterialDialog *dialog = new MaterialDialog(this);
    dialog->set(materialModel, PPLib::addRole, ui->table->currentIndex());

    dialog->exec();
    delete dialog;
}

void StockWidget::rollCut()
{
    Stock *stock = (Stock *) ui->table->currentEntity();
    if (stock == NULL) {
        return;
    }

    CutRollDialog *dialog = new CutRollDialog(ui->table->model(), stock);
    dialog->exec();
    delete dialog;
}

void StockWidget::woffMaterial()
{
    Stock *stock = (Stock *) ui->table->currentEntity();
    if (stock == NULL) {
        return;
    }

    WoffDialog *dialog = new WoffDialog(this);
    dialog->setModel(ui->table->model(), stock);
    dialog->exec();
    delete dialog;
}

void StockWidget::on_checkBox_toggled(bool checked)
{
    ui->frameProduct->setEnabled(checked);

    if (checked) {
        stockF->setFilterRole(PPLib::idRole);

    } else {
        stockF->setFilterRole(-1);
        ui->frameProduct->setDisabled(true);
    }
}

void StockWidget::productChanged(Product *product)
{
    stockF->setMatchValue(product->getId());
}

void StockWidget::appendMenu()
{
    QMenu *menu = ui->table->getMenu();

    connect(ui->table->actionAdd, SIGNAL(triggered()),this, SLOT(addStock()));
    connect(ui->table->actionEdit, SIGNAL(triggered()), this, SLOT(editStock()));
    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editStock(QModelIndex)));

    QAction *actionAddMaterial = new QAction(QString::fromUtf8("Внести материал"), this);
    connect(actionAddMaterial, SIGNAL(triggered()), this, SLOT(addMaterial()));

    QAction *actionWoffMaterial = new QAction(QString::fromUtf8("Списать материал"), this);
    connect(actionWoffMaterial, SIGNAL(triggered()), this, SLOT(woffMaterial()));

    menu->addSeparator();
    menu->addAction(actionAddMaterial);
    menu->addAction(actionWoffMaterial);

    if (stockType == Stock::ROLL){
        QAction *actionRollCut = new QAction(QString::fromUtf8("Резать материал"), this);
        connect(actionRollCut, SIGNAL(triggered()), this, SLOT(rollCut()));
        menu->addAction(actionRollCut);
    }
}
