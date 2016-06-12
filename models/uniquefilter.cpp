#include "uniquefilter.h"

UniqueFilter::UniqueFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void UniqueFilter::setSourceModel(QAbstractItemModel *sourceModel)
{
    connect(sourceModel, SIGNAL(modelReset()), this, SLOT(clearCache()));
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

bool UniqueFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    int fRole = filterRole();
    QAbstractItemModel *model = sourceModel();
    QModelIndex index = model->index(sourceRow, 0);

    const QString &text = model->data(index, fRole).toString();
    if (cache.contains(text)) {
        return false;
    }
    cache << text;
    return true;
}
