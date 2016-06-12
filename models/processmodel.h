#ifndef PROCESSMODEL_H
#define PROCESSMODEL_H

#include <QAbstractListModel>
#include <QBitArray>
#include "utility/process.h"

class ProcessModel : public QAbstractListModel
{
private:
    Process *process;
    QList<int> processes;
    bool check;
    QBitArray cheks;

public:
    explicit ProcessModel(QObject *parent, int condition);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setCheck(bool check);

    int getProcesses();
    void setProcesses(int value);
};

#endif // PROCESSMODEL_H
