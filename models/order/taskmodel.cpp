#include "taskmodel.h"

const QStringList TaskModel::header = QStringList() << "1";
const int TaskModel::workIdRole = PPLib::userRole + 1;

TaskModel::TaskModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0) {
        return QVariant();
    }
    Task *task = static_cast<Task*>(getEntity(row));

    if (role == workIdRole) {
        return task->fields["work_id"];
    }
    return EntityModel::data(index, role);
}

Task *TaskModel::createEntity() const
{
    return new Task();
}

bool TaskModel::update(QAbstractItemModel *model, int workId)
{
    for (int row = 0; row < model->rowCount(); row++){
        QModelIndex indx = model->index(row, 0);
        Task *task = (Task *) model->data(indx, PPLib::entityRole).toInt();

        task->fields["work_id"] = workId;
        if (!task->update()){
            return false;
        }
    }
    return true;
}
