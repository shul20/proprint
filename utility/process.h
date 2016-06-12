#ifndef PROCESS_H
#define PROCESS_H

#include <QMap>
#include <QVariantList>
#include <QVector>

class Process
{
private:
    Process();

    QMap<int, QVector<QVariant> > data;


public:
    static Process *instance;
    static Process *getInstance();

    const QVariant &getName(int type) const;
    const QVariant &getUnits(int type) const;
    int getWeight(int type);
    QVariant &getColor(int type);

    void getProcesses(QList<int> &list, int condition);
    int amount() const;
    QString getProcessStr(int processes);

    const static enum {
        PRINT = 0, LUVERS = 1, LAMINAT = 2, MOUNT = 3, WELD = 4,
        CUT = 5, CONTOUR = 6, POCKET = 7, FINISHING = 8, OTHER = 9, COLORPROOF = 10
    } typeEnum;

    const static enum {
        NAME = 0, UNIT = 1, WEIGHT = 2, COLOR = 3,
        ISPRODUCT = 4, ISMAINPROCESS = 5, ISPRICE = 6, ISEQUIP = 7,
        ISORDER = 8
    } fieldEnum;
};

#endif // PROCESS_H
