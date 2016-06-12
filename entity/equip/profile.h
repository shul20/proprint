#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include "entity/entity.h"

class Profile : public Entity
{
private:
    static const QString table;

public:
    static const char allQuery[];
    static const char passQuery[];

    Profile();
    Profile(Profile *other);

    bool update();

};

#endif // PROFILE_H
