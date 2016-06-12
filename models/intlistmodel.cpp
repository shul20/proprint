#include "intlistmodel.h"
#include <QDebug>

IntListModel::IntListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant IntListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::DisplayRole || role == Qt::UserRole) {
        return list.at(index.row());
    }

    return QVariant();
}

int IntListModel::rowCount(const QModelIndex &parent) const
{
    return list.size();
}


void IntListModel::appendInt(int value)
{
    list.append(value);
    reset();
}

void IntListModel::clear()
{
    list.clear();
    reset();
}
