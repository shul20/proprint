#ifndef VAR_H
#define VAR_H

#include <QStringList>
#include "entity/entity.h"

class Var : public Entity
{
private:
    static const QString table;

public:
    Var();
    Var(Var *other);

    static enum {RATE = 0, OVERRUN = 1, OREFRESH = 2, TREFRESH = 3,
                VARREFRESH = 4, VERSION = 5} varEnum;
    static const QStringList vars;
    static const char allQuery[];

};

#endif // VAR_H
