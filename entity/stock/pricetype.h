#ifndef PRICETYPE_H
#define PRICETYPE_H

#include <QStringList>
#include "entity/entity.h"

class PriceType : public Entity
{
private:
    static const QString table;

public:
    static const char allQuery[];

    PriceType();
    PriceType(PriceType *other);

};

#endif // PRICETYPE_H
