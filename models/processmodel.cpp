#include "processmodel.h"
#include <QColor>
#include <QBrush>
#include "proprintlib.h"

ProcessModel::ProcessModel(QObject *parent, int condition) :
    QAbstractListModel(parent), check(false)
{
    process = Process::getInstance();
    cheks = QBitArray(PPLib::maxProcess);
    process->getProcesses(processes, condition);
}


QVariant ProcessModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int processId = processes.at(row);

    if(role == Qt::DisplayRole){
        return process->getName(processId);
    }

    if(role == PPLib::idRole){
        return processId;
    }

    if (check && (role == Qt::CheckStateRole)) {
            return cheks[processId];
    }

    if (check && (role == Qt::BackgroundColorRole) && cheks[processId]){
            return QBrush(QColor("#FFB2B2"));
        }

    return QVariant();
}

int ProcessModel::rowCount(const QModelIndex &parent) const
{
    return processes.size();
}

Qt::ItemFlags ProcessModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags | Qt::ItemIsUserCheckable;
}

bool ProcessModel::setData(const QModelIndex &index, const QVariant &value, int role){
    int row = index.row();
    int processId = processes.at(row);
    if(role == Qt::CheckStateRole) {
        cheks[processId] = !cheks[processId];

        return true;
    }
    return QAbstractItemModel::setData(index, value, role);

}

void ProcessModel::setCheck(bool check)
{
    this->check = check;
}

int ProcessModel::getProcesses()
{
    int value = 0;
        for(int i = 0; i < cheks.size(); i++) {
            value |= (int)cheks.at(i) << i;
        }
        return value;
}

void ProcessModel::setProcesses(int value){
    for(int i = 0; i < cheks.size(); i++) {
        cheks[i] = (value >> i) & 1;
    }
    reset();
}
