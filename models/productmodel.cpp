#include "productmodel.h"
#include "entity/stock/stock.h"

int ProductModel::processRole = PPLib::userRole + 1;
int ProductModel::stockRole = PPLib::userRole + 2;

const QStringList ProductModel::header =
        QStringList() << QString::fromUtf8("Имя")
                      << QString::fromUtf8("Склад")
                      << QString::fromUtf8("Процессы");

ProductModel::ProductModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant ProductModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    int row = index.row();

    Product *product = static_cast<Product *>(getEntity(row));

    if (role == Qt::DisplayRole && column == 0) {
        return product->fields["product_name"];
    }

    if (role == Qt::DisplayRole && column == 1) {
        return Stock::stocks[product->fields["product_stock"].toInt()];
    }

    if (role == Qt::DisplayRole && column == 2) {
        return product->getProcessesStr();
    }

    if (role == processRole) {
        return product->fields["product_processes"];
    }

    if (role == stockRole) {
        return product->fields["product_stock"];
    }

    return EntityModel::data(index, role);
}

Product *ProductModel::createEntity() const
{
    return new Product();
}
