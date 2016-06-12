#include "stockmodel.h"
#include "utility/size.h"
#include "proprintlib.h"

const int StockModel::paramRole = PPLib::userRole + 1;
const int StockModel::amountRole = PPLib::userRole + 3;
const int StockModel::productIdRole = PPLib::userRole + 4;
const int StockModel::stockTypeRole = PPLib::userRole + 5;

const QStringList StockModel::header = QStringList()
        << QString::fromUtf8("Продукция")
        << QString::fromUtf8("Материал")
        << QString::fromUtf8("Параметр")
        << QString::fromUtf8("Количество")
        << QString::fromUtf8("Склад");

StockModel::StockModel(QObject *parent) :
    EntityModel(parent)
{
    QSqlQuery &query = refreshQuery;
    query.prepare(Stock::allQuery);

    entityHeader = &header;
}

QVariant StockModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    int row = index.row();

    Stock *stock = static_cast<Stock*>(getEntity(row));

    if (role == Qt::DisplayRole && (column == PRODUCT)) {
        return stock->getProductName();
    }

    if ((role == Qt::DisplayRole && (column == NAME)) ||
        (role == PPLib::uniqueRole)) {
        return stock->fields["stock_name"];
    }

    if ((role == Qt::DisplayRole && (column == PARAMETR)) ||
            (role == paramRole)) {
        return stock->fields["stock_param"];
    }

    if (role == Qt::DisplayRole && (column == AMOUNT)) {
        return stock->fields["stock_amount"];
    }

    if (role == Qt::DisplayRole && (column == STOCK)) {
        return stock->fields["stock_type"];
    }

    if (role == stockTypeRole){
        return stock->fields["stock_type"];
    }

    if (role == productIdRole){
        return stock->fields["product_id"].toInt();
    }

    return EntityModel::data(index, role);
}

Stock *StockModel::createEntity() const
{
    return new Stock();
}
