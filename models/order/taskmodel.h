#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/order/task.h"

class TaskModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit TaskModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Task *createEntity() const;
    static bool update(QAbstractItemModel *model, int workId);

    static const int workIdRole;
signals:
    
public slots:
    
};

#endif // TASKMODEL_H
