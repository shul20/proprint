#include "supplyreportm.h"

const QString SupplyReportM::queryStr = QString(
    "select product_name, stock_name, stock_param, material_total, material_debt "
    "from material_view where "
    "(date(material_receipt) between :from and :till) and contact_id in "
    "(select contact_id from contact where company_id = :id)");



const QStringList SupplyReportM::header = QStringList()
        << QString::fromUtf8("Продукция")
        << QString::fromUtf8("Материал")
        << QString::fromUtf8("Параметр")
        << QString::fromUtf8("Всего, грн.")
        << QString::fromUtf8("Долг, грн.");

SupplyReportM::SupplyReportM(QObject *parent) :
    ReportModel(parent)
{
    ReportModel::header = &header;
    query.prepare(queryStr);
}

QVariant SupplyReportM::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            ((column == 2))) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            ((column == 3) || (column == 4) || (column == 6))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    if (role == Qt::DisplayRole && (column == 3)) {
        Money total(ReportModel::data(index, role));
        return total.toString();
    }

    if (role == Qt::DisplayRole && (column == 4)) {
        Money debt(ReportModel::data(index, role));
        return debt.toString();
    }

    return ReportModel::data(index, role);
}

void SupplyReportM::getTotal(Money &total, Money &debt)
{
    for (int i = 0; i < rowCount(); i++){
        int value;

        value = ReportModel::data(index(i, 3), Qt::DisplayRole).toInt();
        total = total + Money(value);

        value = ReportModel::data(index(i, 4), Qt::DisplayRole).toInt();
        debt = debt + Money(value);
    }


}
