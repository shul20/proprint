#ifndef PAY_H
#define PAY_H

#include <QStringList>
#include "entity/entity.h"

class Pay : public Entity
{
private:
    static const QString table;

public:
    Pay();
    Pay(Pay *other);

    static const char allQuery[];
    static const QStringList types;
    static const QStringList credits;
    static const QStringList deposits;

    static const enum {ORDER = 0, MATERIAL = 1, STOCK = 2, OTHER = 3} typeEnum;
    static const enum {CREDIT = 0, DEBET = 1} creditEnum;
    static const enum {NODEBT = 0, CREATEDEBT = 1,
                CHANGEDEBT = 2, WOFFDEBT = 3} debtEnum;
};

#endif // PAY_H
