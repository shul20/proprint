#ifndef TASKSPRINTM_H
#define TASKSPRINTM_H

#include <QAbstractTableModel>
#include <QStringList>
#include "models/equip/equipmentmodel.h"
#include "entity/order/taskview.h"

class TasksPrintM : public QAbstractTableModel
{
    Q_OBJECT
private:
     static const QStringList header;
     EquipmentModel *equipModel;
     QSqlQuery query;

     QMap<int, int> tableMap;
     QMap<int, Entity *> equipMap;
     QMap<int, Entity *> tasks;

     TaskView *createEntity() const;

public:
    explicit TasksPrintM(QObject *parent);
    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                                     int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool isPrinterBusy(int equipId);
    QMap<int, Entity *> &getTasks();
    void clearTasks();

public slots:
    void refresh();
    
private slots:
    void equipReset();

};

#endif // TASKSPRINTM_H
