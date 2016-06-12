#ifndef PMODE_H
#define PMODE_H

#include <QString>
#include "entity/entity.h"

class Pmode : public Entity
{
private:
    static const QString table;

public:
    Pmode();
    Pmode(Pmode *other);

    bool update();

    static const char allQuery[];
    static const char equipQuery[];
};

#endif // PMODE_H
