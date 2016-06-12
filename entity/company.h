#ifndef COMPANY_H
#define COMPANY_H

#include <QStringList>
#include "entity/entity.h"

class Company : public Entity
{
private:
    const static QString table;

public:
    Company();
    Company(Company *other);

    const QString &getTypeName();

    static const QStringList companies;
    static enum {CUSTOMER = 0, SUPPLIER = 1, SUBORDER = 2} typeEnum;

    static const char allQuery[], typeQuery[], fuzzyQuery[];

    static bool updateDebt(int companyId, int total);
};

#endif // COMPANY_H
