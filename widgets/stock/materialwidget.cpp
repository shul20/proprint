#include "materialwidget.h"
#include "ui_materialwidget.h"
#include "entity/client.h"
#include "models/stock/materialmodel.h"
#include "proprintlib.h"
#include "entity/stock/stock.h"
#include "views/stock/stockview.h"
#include "widgets/entitytable.h"
#include "mainwindow.h"
#include "dialogs/finance/paymentdialog.h"
#include "entity/finance/pay.h"
#include "models/modelmanager.h"

MaterialWidget::MaterialWidget(QWidget *parent) :
    QWidget(parent), model(NULL),
    ui(new Ui::MaterialWidget)
{
    ui->setupUi(this);

    EntityTable::setup(ui->table);
    ui->table->setSortingEnabled(false);
    ui->table->setStyleSheet("QTableView::item {margin-left: 20px; margin-right: 20px}");
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->stock->blockSignals(true);
    int size = Stock::stocks.length()-1;
    for (int i = 0; i < size; i++){
        ui->stock->addItem(Stock::stocks[i]);
    }

    endDate = QDate::currentDate();
    startDate = endDate.addMonths(-1);

    ui->startDate->blockSignals(true);
    ui->startDate->setDate(startDate);
    ui->endDate->setDate(endDate);
    ui->startDate->blockSignals(false);
    ui->stock->blockSignals(false);

    appendMenu();
}

MaterialWidget::~MaterialWidget()
{
    delete ui;
}

QAbstractItemModel *MaterialWidget::getModel()
{
    return ui->table->model();
}

int MaterialWidget::currentStock()
{
    return ui->stock->currentIndex();
}

void MaterialWidget::on_stock_currentIndexChanged(int index)
{
    if (model != NULL){
        delete model;
    }
    model = new MaterialModel(this);
    model->setStockType(index);

    QSqlQuery &query = model->getRefreshQuery();
    query.prepare(Material::allQuery);
    query.bindValue(":type", index);
    query.bindValue(":start", ui->startDate->date());
    query.bindValue(":end", ui->endDate->date());
    model->refresh();
    //qDebug() << query.lastQuery();
    ui->table->setModel(model);
}

void MaterialWidget::on_startDate_dateChanged(const QDate &date)
{
    startDate = date;
    on_stock_currentIndexChanged(ui->stock->currentIndex());
}

void MaterialWidget::on_endDate_dateChanged(const QDate &date)
{
    endDate = date;
    on_stock_currentIndexChanged(ui->stock->currentIndex());
}

void MaterialWidget::editMaterial()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }
    editMaterial(ui->table->currentIndex());
}


void MaterialWidget::editMaterial(QModelIndex index)
{
    MaterialDialog *dialog = new MaterialDialog(this);
    dialog->set(model, Qt::EditRole, index);
    dialog->exec();

    delete dialog;
}

void MaterialWidget::delMaterial()
{
    QString message;
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    int result = QMessageBox::question(this, QString::fromUtf8("Удаление"),
               QString::fromUtf8("Удалить материал?"), QMessageBox::Cancel | QMessageBox::Ok);

    if (result == QMessageBox::Cancel) {
        return;
    }

    QModelIndex index = ui->table->currentIndex();
    Material * material = (Material *)model->data(index, PPLib::entityRole).toInt();
    int stockId = material->fields["stock_id"].toInt();

    MainWindow::db->transaction();
    if (!model->setData(index, true, PPLib::delRole)) {

        MainWindow::db->rollback();
        return;
    }

    MainWindow::db->commit();

    StockModel *stockModel = (StockModel *) ModelManager::getInstance()->getStockModel();
    Stock *stock = (Stock *)stockModel->findEntity(stockId);
    if (stock == NULL) {
        return;
    }
    stock->retrive(stockId, false);
}

void MaterialWidget::addPayment()
{
    if (!ui->table->selectionModel()->hasSelection()){
        return;
    }

    Entity * materialView = (Entity *)model->data(ui->table->currentIndex(),
                                     PPLib::entityRole).toInt();

    PaymentDialog *dialog = new PaymentDialog(this);
    dialog->setModel(NULL, PaymentDialog::MATERIAL, materialView);
    dialog->exec();
    delete dialog;
}

void MaterialWidget::appendMenu()
{
    menu = new QMenu(this);

    QAction *actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), model, SLOT(refresh()));


    if (Client::currentRole == Client::ADMIN){
        QAction *actionEdit = new QAction(QString::fromUtf8("Изменить"), this);
        connect(actionEdit, SIGNAL(triggered()), this, SLOT(editMaterial()));
        connect(ui->table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editMaterial(QModelIndex)));

        QAction *actionDel = new QAction(QString::fromUtf8("Удалить"), this);
        connect(actionDel, SIGNAL(triggered()), this, SLOT(delMaterial()));

        menu->addAction(actionEdit);
        menu->addAction(actionDel);

    }

    menu->addAction(actionRefresh);

    QAction *actionAddPayment = new QAction(QString::fromUtf8("Внести проплату"), this);
    connect(actionAddPayment, SIGNAL(triggered()), this, SLOT(addPayment()));

    menu->addSeparator();
    menu->addAction(actionAddPayment);

}

void MaterialWidget::on_table_customContextMenuRequested(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}
