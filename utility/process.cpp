#include "process.h"
#include <QDebug>
#include "proprintlib.h"

Process *Process::instance = NULL;

Process::Process()
{
    data[PRINT] = QVector<QVariant>()
        << QString::fromUtf8("Печать") << QString::fromUtf8("кв. м")
        << 10 << QString("0,85,255,35")
        << true << true << true << true << true;

    data[LUVERS] = QVector<QVariant>()
        << QString::fromUtf8("Люверсы") << QString::fromUtf8("шт.")
        << 20 << QString("0,170,127,45")
        << true << true << true << false << false;

    data[LAMINAT] = QVector<QVariant>()
        << QString::fromUtf8("Ламинация") << QString::fromUtf8("кв. м")
        << 30 << QString("170,0,127,45")
        << true << true << true << true << false;

    data[MOUNT] = QVector<QVariant>()
        << QString::fromUtf8("Прикатка") << QString::fromUtf8("кв. м")
        << 40 << QString("147,64,255,55")
        << true << true << true << false << false;

    data[CUT] = QVector<QVariant>()
        << QString::fromUtf8("Порезка") << QString::fromUtf8("м")
        << 50 << QString("10,198,255,55")
        << true << true << true << false << false;

    data[POCKET] = QVector<QVariant>()
        << QString::fromUtf8("Карман") << QString::fromUtf8("м")
        << 60 << QString("46,84,255,45")
        << true << true << true << false << false;

    data[WELD] = QVector<QVariant>()
        << QString::fromUtf8("Сварка") << QString::fromUtf8("м")
        << 60 << QString("255,108,10,45")
        << true << true << true << false << false;

    data[FINISHING] = QVector<QVariant>()
        << QString::fromUtf8("Обработка") << QString::fromUtf8("м")
        << 70 << QString("0,250,196,45")
        << true << true << true << false << false;

    data[OTHER] = QVector<QVariant>()
        << QString::fromUtf8("Другое") << QString::fromUtf8(" ")
        << 0 << QString("46,84,255,45")
        << true << false << false << false << false;

    data[COLORPROOF] = QVector<QVariant>()
        << QString::fromUtf8("Цветопроба") << QString::fromUtf8("кв. м")
        << 0 << QString("226,10,255,45")
        << false << false << false << false << false;


}

Process *Process::getInstance()
{
    if (instance == NULL) {
        instance = new Process();
    }

    return instance;
}

const QVariant &Process::getName(int type) const
{
    return data[type][NAME];
}

const QVariant &Process::getUnits(int type) const
{
    return data[type][UNIT];
}

int Process::getWeight(int type)
{
    return data[type][WEIGHT].toInt();
}

QVariant &Process::getColor(int type)
{
    return data[type][COLOR];
}

void Process::getProcesses(QList<int> &list, int condition)
{
    QMap<int, QVector<QVariant> >::iterator it;
    for (it = data.begin(); it != data.end(); it++){
        if (it.value()[condition].toBool()){
            list.append(it.key());
        }
    }
}

int Process::amount() const
{
    return data.size();
}

QString Process::getProcessStr(int processes)
{
    QString processesStr;
    QMap<int, QVector<QVariant> >::iterator it;

    for (it = data.begin(); it != data.end(); it++){
        int processId = it.key();
        if (PPLib::checkBit(QVariant(processId), QVariant(processes))) {
            processesStr.append(data[processId][NAME].toString());
            processesStr.append(" ");
        }

    }
    return processesStr;
}
