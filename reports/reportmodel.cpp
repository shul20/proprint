#include "reportmodel.h"

ReportModel::ReportModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

QSqlQuery &ReportModel::getQuery()
{
    return query;
}

QVariant ReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return header->at(section);

    } else {
        return QString("%1").arg( section + 1 );
    }
}

int ReportModel::columnCount(const QModelIndex &parent) const
{
    return header->size();
}
