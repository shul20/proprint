#include "woffview.h"
#include "ui_woffview.h"
#include "entity/stock/stock.h"
#include "models/stock/woffmodel.h"
#include "widgets/entitytable.h"

WoffView::WoffView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WoffView)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("Списание материалов"));

    ui->table->setSortingEnabled(false);
    EntityTable::setup(ui->table);
    setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");

    ui->stock->blockSignals(true);
    int size = Stock::stocks.length()-1;
    for (int i = 0; i < size; i++){
        ui->stock->addItem(Stock::stocks[i]);
    }

    dateTill = QDate::currentDate();
    dateFrom = dateTill.addMonths(-1);

    ui->dateFrom->blockSignals(true);
    ui->dateFrom->setDate(dateFrom);
    ui->dateTill->setDate(dateTill);
    ui->dateFrom->blockSignals(false);
    ui->stock->blockSignals(false);

    appendMenu();

}

WoffView::~WoffView()
{
    delete ui;
}

void WoffView::on_stock_currentIndexChanged(int index)
{
    WoffModel *model = (WoffModel *) ui->table->model();

    if (model != NULL){
        delete model;
    }
    model = new WoffModel(this);
    model->setStockType(index);

    QSqlQuery &query = model->getRefreshQuery();
    query.prepare(Woff::allByStock);
    query.bindValue(":stock", index);
    query.bindValue(":start", ui->dateFrom->date());
    query.bindValue(":end", ui->dateTill->date());
    model->refresh();

    ui->table->setModel(model);
}

void WoffView::on_dateFrom_dateChanged(const QDate &date)
{
    dateFrom = date;
    on_stock_currentIndexChanged(ui->stock->currentIndex());
}

void WoffView::on_dateTill_dateChanged(const QDate &date)
{
    dateTill = date;
    on_stock_currentIndexChanged(ui->stock->currentIndex());
}

void WoffView::appendMenu()
{
    menu = new QMenu(this);

    QAction *actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refresh()));
    menu->addAction(actionRefresh);
}

void WoffView::on_table_customContextMenuRequested(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}

void WoffView::refresh()
{
    ui->table->model()->setData(QModelIndex(), QVariant(), PPLib::refreshRole);
}
