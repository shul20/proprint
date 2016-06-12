#ifndef TASKPOSTM_H
#define TASKPOSTM_H

#include <QAbstractTableModel>
#include <QStringList>
#include "entity/order/taskview.h"

class TaskPostM : public QAbstractTableModel
{
    Q_OBJECT
private:
     static const QStringList header;
     QList<Entity *> tasks;

     TaskView *createEntity() const;

public:
    explicit TaskPostM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                                     int role = Qt::DisplayRole) const;

    QList<Entity *> &getTasks();
    void clearTasks();
    
public slots:
    void refresh();
    
};

#endif // TASKPOSTM_H
