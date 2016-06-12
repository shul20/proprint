#ifndef PASS_H
#define PASS_H

#include <QString>
#include "entity/entity.h"

class Pass : public Entity
{
private:
    static const QString table;

public:
    static const char allQuery[];
    static const char modeQuery[];

    Pass();
    Pass(Pass *other);

    bool update();

};

#endif // PASS_H
