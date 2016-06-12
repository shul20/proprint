#include "customerreportm.h"
#include "entity/company.h"

const QString CustomerReportM::queryStr = QString(
    "select ordr_id, product_name, stock_name, client_name, ordr_size, ordr_total, ordr_debt "
    "from ordr_viewarch where "
    "(date(ordr_release) between :from and :till) and contact_id in "
    "(select contact_id from contact where company_id = :id)");



const QStringList CustomerReportM::header = QStringList()
        << QString::fromUtf8("№")
        << QString::fromUtf8("Продукция")
        << QString::fromUtf8("Материал")
        << QString::fromUtf8("Менеджер")
        << QString::fromUtf8("кв.м")
        << QString::fromUtf8("Всего, грн.")
        << QString::fromUtf8("Долг, грн.");

CustomerReportM::CustomerReportM(QObject *parent) :
    ReportModel(parent)
{
    ReportModel::header = &header;
    query.prepare(queryStr);
}


QVariant CustomerReportM::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            ((column == 9))) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            ((column == 4) || (column == 5) || (column == 6))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    if (role == Qt::DisplayRole && (column == 4)) {
        QVariant size = ReportModel::data(index, role);
        return Size(size).toString();
    }

    if (role == Qt::DisplayRole && (column == 5)) {
        Money total(ReportModel::data(index, role));
        return total.toString();
    }

    if (role == Qt::DisplayRole && (column == 6)) {
        Money debt(ReportModel::data(index, role));
        return debt.toString();
    }

    return ReportModel::data(index, role);
}

void CustomerReportM::getTotal(Money &total, Money &debt, Size &size)
{
    for (int i = 0; i < rowCount(); i++){
        int value;

        value = ReportModel::data(index(i, 4), Qt::DisplayRole).toInt();
        size += Size(value);

        value = ReportModel::data(index(i, 5), Qt::DisplayRole).toInt();
        total = total + Money(value);

        value = ReportModel::data(index(i, 6), Qt::DisplayRole).toInt();
        debt = debt + Money(value);
    }

}

