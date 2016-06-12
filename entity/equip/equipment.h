#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QStringList>
#include "entity/entity.h"

class Equipment : public Entity
{
private:
    static const QString table;

public:
    Equipment();
    Equipment(Equipment *other);
    ~Equipment();

    static const char allQuery[];
    const static enum { WORK = 0, CONSERVED = 1, REPAIR = 2 } enumStatus;
    static const QStringList statuses;
    static const QList<QColor> colors;
};

#endif // EQUIPMENT_H
