#include "taskpostm.h"
#include "utility/process.h"
#include "utility/size.h"
#include <QDateTime>

const QStringList TaskPostM::header =
        QStringList() << QString::fromUtf8("Процесс")
                      << QString::fromUtf8("№")
                      << QString::fromUtf8("Заказчик")
                      << QString::fromUtf8("Продукция")
                      << QString::fromUtf8("Материал")
                      << QString::fromUtf8("кв.м")
                      << QString::fromUtf8("Прогноз")
                      << QString::fromUtf8("Отдать")
                      << QString::fromUtf8("Оператор")
                      << QString::fromUtf8("Менеджер");

TaskPostM::TaskPostM(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QVariant TaskPostM::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();
    if (role == Qt::TextAlignmentRole &&
            (column == 5)) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }
    if (role == Qt::TextAlignmentRole &&
            (column == 6 || column == 7 || column == 8 || column == 9)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    TaskView *task = (TaskView *) tasks.at(row);
    if (task == NULL) {
        return QVariant();
    }

    if (role == Qt::DisplayRole && column == 0) {
        int processId = task->fields["task_process"].toInt();
        return Process::getInstance()->getName(processId);
    }
    if (role == Qt::DisplayRole && column == 1) {
        return task->fields["ordr_id"];
    }
    if (role == Qt::DisplayRole && column == 2) {
        return task->fields["company_name"];
    }
    if (role == Qt::DisplayRole && column == 3) {
        return task->fields["product_name"];
    }
    if (role == Qt::DisplayRole && column == 4) {
        return task->fields["stock_name"];
    }
    if (role == Qt::DisplayRole && column == 5) {
        return Size(task->fields["ordr_size"]).toString();
    }
    if (role == Qt::DisplayRole && column == 6) {
        return task->fields["ordr_predict"].
                toDateTime().toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && column == 7) {
        return task->fields["ordr_end"].
                toDateTime().toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && column == 8) {
        return task->fields["who_operate"];
    }
    if (role == Qt::DisplayRole && column == 9) {
        return task->fields["who_manager"];
    }
    if (role == PPLib::entityRole) {
        return (int) task;
    }
    return QVariant();
}

int TaskPostM::rowCount(const QModelIndex &parent) const
{
    return tasks.size();
}

int TaskPostM::columnCount(const QModelIndex &parent) const
{
    return header.size();
}

QVariant TaskPostM::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return header.at(section);

    } else {
        return QString("%1").arg( section + 1 );
    }
}

QList<Entity *> &TaskPostM::getTasks()
{
    return tasks;
}

void TaskPostM::clearTasks()
{
    foreach (Entity *task, tasks) {
        delete task;
    }
    tasks.clear();
}

void TaskPostM::refresh()
{
    reset();
}
