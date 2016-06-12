#include "client.h"
#include <QDebug>

Client::Client():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Client::Client(Client *other): Entity(other)
{}

const QString &Client::getRoleName()
{
    int role = fields["client_role"].toInt();
    return roles[role];
}

bool Client::isOwner(int clientId)
{
    if (currentRole == ADMIN || clientId == currentClient->getId()) {
        return true;
    }
    if ((currentRole == PRINT || currentRole == POSTPRINT) &&
        clientId == 0) {
        return true;
    }
    return false;
}


const QString Client::table("client");
Client *Client::currentClient = NULL;
int Client::currentRole = -1;

const char Client::allQuery[] = "select * from client;";

const QStringList Client::roles = QStringList()
                          << QString::fromUtf8("Администратор")
                          << QString::fromUtf8("Менеджер")
                          << QString::fromUtf8("Печатник")
                          << QString::fromUtf8("Постпечатник");
