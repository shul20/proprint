#include "sortfiltermodel.h"
#include <QDebug>
#include "proprintlib.h"

SortFilterModel::SortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    compare = &SortFilterModel::equal;

}

bool SortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    int fRole = filterRole();
    if (fRole < 0) {
        return true;
    }

    QAbstractItemModel *model = sourceModel();
    QModelIndex index = model->index(source_row, 0);

    return compare(matchValue, model->data(index, fRole));
}

QVariant SortFilterModel::getMatchValue() const
{
    return matchValue;
}

void SortFilterModel::setMatchValue(const QVariant &value)
{
    matchValue = value;
    filterChanged();
    //reset();
}


bool SortFilterModel::equal(QVariant filter, QVariant dataFromModel)
{
    return filter == dataFromModel;
}

bool SortFilterModel::contain(QVariant filter, QVariant dataFromModel)
{
    QString find = filter.toString();
    QString data = dataFromModel.toString();

    return data.contains(find, Qt::CaseInsensitive);
}

bool SortFilterModel::paramFilter(QVariant filter, QVariant dataFromModel)
{
    Entity *e1 = (Entity *) dataFromModel.toInt();
    Entity *e2 = (Entity *) filter.toInt();
    if (e1 == NULL || e2 == NULL) {
        return false;
    }

    return e1->fields["product_id"] == e2->fields["product_id"] &&
           e1->fields["stock_name"] == e2->fields["stock_name"];
}


