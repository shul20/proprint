#ifndef CONTACT_H
#define CONTACT_H

#include "entity/entity.h"
#include "utility/money.h"

class Contact : public Entity
{
private:
    static const QString table;
    Money debt;

public:
    Contact();
    Contact(Contact *other);

    bool update();
    static const char allQuery[], allByCompanyQuery[], allByCompanyTypeQuery[];

    static bool updateDebt(int contactId, int total);
    static int getCompanyId(int contactId);

};

#endif // CONTACT_H
