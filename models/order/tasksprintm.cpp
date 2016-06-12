#include "tasksprintm.h"
#include "models/modelmanager.h"
#include "models/equip/equipmentmodel.h"
#include "utility/size.h"
#include "utility/process.h"
#include "entity/order/task.h"
#include <QDateTime>

const QStringList TasksPrintM::header =
        QStringList() << QString::fromUtf8("Оборудование")
                      << QString::fromUtf8("Состояние")
                      << QString::fromUtf8("№")
                      << QString::fromUtf8("Заказчик")
                      << QString::fromUtf8("Продукция")
                      << QString::fromUtf8("Материал")
                      << QString::fromUtf8("кв.м")
                      << QString::fromUtf8("Прогноз")
                      << QString::fromUtf8("Отдать")
                      << QString::fromUtf8("Оператор")
                      << QString::fromUtf8("Менеджер");

TasksPrintM::TasksPrintM(QObject *parent) :
    QAbstractTableModel(parent)
{    
    equipModel = ModelManager::getInstance()->getEquipModel();
    connect(equipModel, SIGNAL(modelReset()), this, SLOT(equipReset()));
    equipReset();
}

int TasksPrintM::rowCount(const QModelIndex &parent) const
{
    return tableMap.size();
}

int TasksPrintM::columnCount(const QModelIndex &parent) const
{
    return header.size();
}

QVariant TasksPrintM::headerData(int section, Qt::Orientation orientation, int role) const
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

bool TasksPrintM::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        TaskView *task = (TaskView *) value.toInt();
        int equipId = task->fields["equipment_id"].toInt();
        tasks[equipId] = task;
        reset();
        return true;
    }
    return QAbstractTableModel::setData(index, value, role);
}

bool TasksPrintM::isPrinterBusy(int equipId)
{
    return tasks.contains(equipId);
}

QMap<int, Entity *> &TasksPrintM::getTasks()
{
    return tasks;
}

void TasksPrintM::refresh()
{    
    reset();
}

QVariant TasksPrintM::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            (column == 9 || column == 10)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            (column == 6)) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    Equipment *equip = (Equipment *) equipMap[row];
    if (equip == NULL) {
        return QVariant();
    }

    int status = equip->fields["equipment_status"].toInt();

    if (role == Qt::BackgroundRole) {
        return QBrush(Equipment::colors[status]);
    }

    if (role == Qt::DisplayRole && column == 0) {
        return equip->fields["equipment_name"];
    }
    if (role == Qt::DisplayRole && column == 1) {
            return Equipment::statuses[status];
    }

    Entity *task = tasks[equip->getId()];
    if (task == NULL) {
        return QVariant();
    }

    if (role == Qt::DisplayRole && column == 2) {
        return task->fields["ordr_id"];
    }
    if (role == Qt::DisplayRole && column == 3) {
        return task->fields["company_name"];
    }
    if (role == Qt::DisplayRole && column == 4) {
        return task->fields["product_name"];
    }
    if (role == Qt::DisplayRole && column == 5) {
        return task->fields["stock_name"];
    }
    if (role == Qt::DisplayRole && column == 6) {
        return Size(task->fields["ordr_size"]).toString();
    }
    if (role == Qt::DisplayRole && column == 7) {
        return task->fields["ordr_predict"].
                toDateTime().toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && column == 8) {
        return task->fields["ordr_end"].
                toDateTime().toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && column == 9) {
        return task->fields["who_operate"];
    }
    if (role == Qt::DisplayRole && column == 10) {
        return task->fields["who_manager"];
    }
    if (role == PPLib::entityRole) {
        return (int) task;
    }
    return QVariant();
}

TaskView *TasksPrintM::createEntity() const
{
    return new TaskView();
}

void TasksPrintM::clearTasks()
{
    foreach (Entity *task, tasks) {
        delete task;
    }
    tasks.clear();
}

void TasksPrintM::equipReset()
{
    equipMap.clear();
    tableMap.clear();

    for(int i = 0, row = 0; i < equipModel->rowCount(); i++){
        QModelIndex index = equipModel->index(i, 0);
        Equipment *equip = (Equipment *) equipModel->data(index, PPLib::entityRole).toInt();
        int process = equip->fields["equipment_process"].toInt();

        if (process == Process::PRINT) {
            equipMap[row] = equip;
            tableMap[row] = equip->getId();
            row += 1;
        }
    }
    reset();
}
