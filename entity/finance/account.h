#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QStringList>
#include "entity/entity.h"

class Account : public Entity
{
private:
    static const QString table;

public:
    Account();
    Account(Account *other);

    static enum { MAIN = 0, DEPOSIT = 1} accountEnum;
    static enum { CACHE = 0, NONCACHE = 1} subAccountEnum;

    static const QStringList accounts;
    static const char allQuery[];
};

#endif // ACCOUNT_H
