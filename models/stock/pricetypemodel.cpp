#include "pricetypemodel.h"

const QStringList PriceTypeModel::header =
        QStringList() << QString::fromUtf8("Прайс");

PriceTypeModel::PriceTypeModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant PriceTypeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if (row < 0) {
        return QVariant();
    }

    PriceType *priceType = static_cast<PriceType *>(getEntity(row));

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return priceType->fields["pricetype_name"];
    }

    return EntityModel::data(index, role);
}


PriceType *PriceTypeModel::createEntity() const
{
    return new PriceType();
}
