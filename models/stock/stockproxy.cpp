#include "stockproxy.h"
#include "models/stock/stockmodel.h"
#include "models/modelmanager.h"
#include <QDebug>


StockProxy::StockProxy(QObject *parent, int type) :
    QSortFilterProxyModel(parent), type(type)
{
    header = QStringList()
            << QString::fromUtf8("Продукция")
            << QString::fromUtf8("Материал");

    if (type == Stock::ROLL){
        title = QString::fromUtf8("Рулонный склад");
        header  << QString::fromUtf8("Х, мм")
                << QString::fromUtf8("Рул.")
                << QString::fromUtf8("кв. м");

    } else if (type == Stock::LIST) {
        title = QString::fromUtf8("Листовой склад");
        header  << QString::fromUtf8("H, мм")
                << QString::fromUtf8("Листов");


    } else if (type == Stock::ITEM) {
        title = QString::fromUtf8("Штучный склад");
        header  << QString::fromUtf8("Объем")
                << QString::fromUtf8("Кол-во")
                << QString::fromUtf8("Всего");

    }

    StockModel *stockModel = ModelManager::getInstance()->getStockModel();
    setSourceModel(stockModel);
}

bool StockProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QAbstractItemModel *model = sourceModel();
    QModelIndex index = model->index(source_row, 0);

    return type == model->data(index, StockModel::stockTypeRole).toInt();
}

const QString &StockProxy::getTitle() const
{
    return title;
}

QVariant StockProxy::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return header.at(section);

    } else {
        return QString("%1").arg( section + 1 );
    }

}

QVariant StockProxy::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole && (column == 2)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole && ((column == 3) || (column == 4))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }
    Stock *stock = (Stock *) sourceModel()->
                        data(mapToSource(index), PPLib::entityRole).toInt();

    if (role == Qt::DisplayRole && (column == 3)) {
       if (type == Stock::ROLL) {
           return PPLib::calcRollsAmount(stock->fields["stock_param"].toInt(),
                   stock->fields["stock_amount"].toInt());
        }

       if (type == Stock::LIST) {
           return stock->fields["stock_amount"].toInt();
       }

       if (type == Stock::ITEM) {
           int amount = stock->fields["stock_amount"].toInt();
           int parametr = stock->fields["stock_param"].toInt();
           return amount / parametr;
       }
    }

    if (role == Qt::DisplayRole && (column == 0)) {
        return stock->getProductName();
    }

    if (role == Qt::DisplayRole && (column == 1)) {
        return stock->fields["stock_name"];
    }

    if (role == Qt::DisplayRole && (column == 2)) {
        return stock->fields["stock_param"];
    }

    if (role == Qt::DisplayRole && (column == 4)) {
        if (type == Stock::ROLL) {
            return Size(stock->fields["stock_amount"]).toString();
        }

        if (type == Stock::ITEM) {
            return stock->fields["stock_amount"].toInt();
        }
    }

    if (role == PPLib::idRole) {
        return stock->fields["product_id"];
    }

    if (role == PPLib::entityRole) {
        return (int)stock;
    }

    return QVariant();
}


int StockProxy::columnCount(const QModelIndex &parent) const
{
    return header.size();
}
