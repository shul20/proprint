#ifndef TASK_H
#define TASK_H

#include <QStringList>
#include "entity/entity.h"
#include "utility/size.h"

class Task : public Entity
{
private:
    static const QString table;
    Entity *woff;
    bool isReady;

    void setFields(const QSqlRecord &record);

public:
    Task();
    Task(Task *other);

    Entity *getWoff();
    void setWoff(Entity *entity);
    bool getIsReady();
    void setIsReady(bool value);

    static const char allByWorkQuery[], allByOrderQuery[];
    static const QStringList statuses;
    static const QVector<QColor> statusesColor;
    static const enum {
        READY = 0,  PREPARE = 1, PROCESS = 2, PAUSE = 3 } statusEnum;

    static const QStringList typeList;
    static const enum {UNI = 0, BIDI = 1, DOUBLESIDE = 2} printType;

    static Size getSizeOfTasks(int stockId, int taskStatus);
};

#endif // TASK_H
