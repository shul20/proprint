#include "pricemodel.h"
#include <QDate>
#include "utility/process.h"
#include "utility/money.h"

const int PriceModel::processRole = PPLib::userRole + 1;
const int PriceModel::companyRole = PPLib::userRole + 2;

QStringList PriceModel::header = QStringList()
        << QString::fromUtf8("Процесс")
        << QString::fromUtf8("Продукция")
        << QString::fromUtf8("Материал")
        << QString::fromUtf8("Цена USD")
        << QString::fromUtf8("Прайс")
        << QString::fromUtf8("Компания")
        << QString::fromUtf8("Дата");

PriceModel::PriceModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant PriceModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            ((column == 2) || (column == 5) || (column == 6))) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            ((column == 3) || (column == 4) || (column == 7) ||
             (column == 8) || (column == 9))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    int row = index.row();
    Price *price = static_cast<Price *>(getEntity(row));

    if (role == Qt::DisplayRole && (column == 0)) {
        int process = price->fields["price_process"].toInt();
        return Process::getInstance()->getName(process);
    }

    if (role == Qt::DisplayRole && (column == 1)) {
        return price->getProduct();
    }

    if (role == Qt::DisplayRole && (column == 2)) {
        return price->fields["price_stock"];
    }

    if (role == Qt::DisplayRole && (column == 4)) {
        return price->getType();
    }

    if (role == Qt::DisplayRole && (column == 3)) {
        Money total = price->fields["price_total"];
        return total.toString();
    }

    if ((role == Qt::DisplayRole && (column == 5)) || role == companyRole) {
        return price->getCompany();
    }

    if (role == Qt::DisplayRole && (column == 6)) {
        return price->fields["price_date"].toDate()
                .toString(PPLib::dateTemplate);
    }

    if (role == processRole) {
        return price->fields["price_process"];
    }

    return EntityModel::data(index, role);
}

Price *PriceModel::createEntity() const
{
    return new Price();
}
