#ifndef CLIENT_H
#define CLIENT_H

#include <QStringList>
#include "entity/entity.h"

class Client : public Entity
{
private:
    static const QString table;

public:
    Client();
    Client(Client *other);

    const QString &getRoleName();

    static Client *currentClient;
    static bool isOwner(int clientId);

    static int currentRole;
    static enum { ADMIN = 0, MANAGER = 1, PRINT = 2, POSTPRINT = 3} roleEnum;
    static const QStringList roles;
    static const char allQuery[];

};

#endif // CLIENT_H
