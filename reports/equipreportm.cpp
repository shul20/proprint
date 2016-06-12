#include "equipreportm.h"
#include "utility/size.h"

const QString EquipReportM::queryStr = QString::fromUtf8(
            "select product_name, stock_name, stock_param, "
            "sum(woff_amount), sum(woff_waste) "
            "from woff_view where task_id in "
            "(select task_id from task where equipment_id = :id) and "
            "(date(woff_date) between :from and :till) "
            "group by product_name, stock_name, stock_param;"
            );

const QStringList EquipReportM::header = QStringList()
        << QString::fromUtf8("Продукция")
        << QString::fromUtf8("Материал")
        << QString::fromUtf8("Ширина, мм")
        << QString::fromUtf8("Расход, м.п.")
        << QString::fromUtf8("Брак, м.п.")
        << QString::fromUtf8("Всего, кв.м.");


EquipReportM::EquipReportM(QObject *parent) :
    ReportModel(parent)
{
    ReportModel::header = &header;
    query.prepare(queryStr);
}

QVariant EquipReportM::data(const QModelIndex &index, int role) const
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

    int row = index.row();
    int amount = QSqlQueryModel::data(this->index(row, 3)).toInt();
    int param = QSqlQueryModel::data(this->index(row, 2)).toInt();
    int waste = QSqlQueryModel::data(this->index(row, 4)).toInt();

    if (role == Qt::DisplayRole && (column == 3)) {

        return Size((amount * 1000) / param).toString();
    }

    if (role == Qt::DisplayRole && (column == 4)) {
        return Size((waste * 1000) / param).toString();
    }

    if (role == Qt::DisplayRole && (column == 5)) {
        return Size(amount).toString();
    }

    if (role == Qt::UserRole && (column == 5)) {
        return amount;
    }

    return QSqlQueryModel::data(index, role);
}

Size EquipReportM::getTotalSize()
{
    Size total;
    for (int i = 0; i < rowCount(); i++){
        total += Size(data(index(i,5), Qt::UserRole));
    }
    return total;
}
