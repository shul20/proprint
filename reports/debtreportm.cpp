#include "debtreportm.h"
#include "entity/company.h"

const QString DebtReportM::queryStr = QString(
            "select company_name, company_type, company_debt "
            "from company where company_debt != 0 and company_type in (%1);");

const QStringList DebtReportM::header = QStringList()
        << QString::fromUtf8("Компания")
        << QString::fromUtf8("Тип")
        << QString::fromUtf8("Долг, грн");

DebtReportM::DebtReportM(QObject *parent) :
    ReportModel(parent)
{
    ReportModel::header = &header;
}

QVariant DebtReportM::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            ((column == 2) || (column == 5) || (column == 6))) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            ((column == 3) || (column == 4) || (column == 7) ||
             (column == 8) || (column == 9) || (column == 10))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    if (role == Qt::DisplayRole && (column == 1)) {
        int type = ReportModel::data(index, role).toInt();
        return Company::companies[type];
    }

    if (role == Qt::DisplayRole && (column == 2)) {
        Money total(ReportModel::data(index, role));
        return total.toString();
    }

    return ReportModel::data(index, role);
}

Money DebtReportM::getTotal()
{
    Money total;
    for (int i = 0; i < rowCount(); i++){
        int cents = abs(ReportModel::data(index(i, 2), Qt::DisplayRole).toInt());
        total = total + Money(cents);
    }
    return total;
}

const QString &DebtReportM::getQueryStr()
{
    return queryStr;
}
