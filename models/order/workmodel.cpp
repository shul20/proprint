#include "workmodel.h"
#include "utility/money.h"
#include "entity/order/task.h"

const QStringList WorkModel::headerForCreate =
        QStringList() << QString::fromUtf8("Ширина, мм")
                      << QString::fromUtf8("Высота, мм")
                      << QString::fromUtf8("Кол-во, шт.")
                      << QString::fromUtf8("Площадь, кв. м")
                      << QString::fromUtf8("Печать, грн")
                      << QString::fromUtf8("Услуги, грн");

const QStringList WorkModel::headerForTask =
        QStringList() << QString::fromUtf8("Ширина, мм")
                      << QString::fromUtf8("Высота, мм")
                      << QString::fromUtf8("Кол-во.")
                      << QString::fromUtf8("кв. м")
                      << QString::fromUtf8("Статус");

WorkModel::WorkModel(QObject *parent) :
    EntityModel(parent)
{
}

void WorkModel::setMode(int mode)
{
    tableMode = mode;
    if (mode == CREATE) {
        entityHeader = &headerForCreate;

    } else if (mode == PROCESS) {
        entityHeader = &headerForTask;
    }
}

QVariant WorkModel::data(const QModelIndex &index, int role) const
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
    Work *work = static_cast<Work*> (getEntity(row));

    if (role == Qt::DisplayRole && (column == 0)) {
        return work->fields["work_width"];
    }
    if (role == Qt::DisplayRole && (column == 1)) {
        return work->fields["work_height"];
    }
    if (role == Qt::DisplayRole && (column == 2)) {
        return work->fields["work_amount"];
    }
    if (role == Qt::DisplayRole && (column == 3)) {
        return Size(work->fields["work_size"]).toString();
    }
    if (role == Qt::DisplayRole && (column == 4)) {
        if (tableMode == CREATE) {
            return Money(work->fields["work_totalprint"]).toString();

        } else if (tableMode == PROCESS) {
            return Task::statuses[work->fields["work_status"].toInt()];
        }
    }
    if (role == Qt::DisplayRole && (column == 5)) {
        if (tableMode == CREATE) {
            Money total = work->fields["work_total"];
            Money totalPrint = work->fields["work_totalprint"];

            return (total - totalPrint).toString();
        }
    }
    if (role == Qt::BackgroundRole && tableMode == PROCESS) {
        int status = work->fields["work_status"].toInt();
        return QBrush(Task::statusesColor[status]);
    }

    return EntityModel::data(index, role);
}

Work *WorkModel::createEntity() const
{
    return new Work();
}

void WorkModel::reset()
{
    EntityModel::reset();
}
