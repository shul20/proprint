#include "stockview.h"
#include "ui_stockview.h"
#include "entity/stock/stock.h"

StockView *StockView::instance = NULL;

StockView::StockView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockView)
{
    ui->setupUi(this);
    instance = this;

    setWindowTitle(QString::fromUtf8("Склады"));

    ui->rollStock->set(Stock::ROLL);
    ui->listStock->set(Stock::LIST);
    ui->itemStock->set(Stock::ITEM);
}

StockView::~StockView()
{
    delete ui;
}

QAbstractItemModel *StockView::getMaterialModel()
{
    return ui->materials->getModel();
}

int StockView::currentMaterialStock()
{
    return ui->materials->currentStock();
}

