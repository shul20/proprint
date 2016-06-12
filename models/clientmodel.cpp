#include "clientmodel.h"
#include <QDebug>

const int ClientModel::roleRole = PPLib::userRole + 1;

const QStringList ClientModel::header =
        QStringList() << QString::fromUtf8("Имя")
                      << QString::fromUtf8("Роль")
                      << QString::fromUtf8("Изменен");

ClientModel::ClientModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;

    QSqlQuery &query = refreshQuery;
    query.prepare(Client::allQuery);
}


QVariant ClientModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole && (column == 2)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    int row = index.row();
    Client *client = static_cast<Client*>(getEntity(row));

    if (role == Qt::DisplayRole && column == 0) {
        return client->fields["client_name"];
    }

    if (role == Qt::DisplayRole && column == 1) {
        return client->getRoleName();
    }

    if (role == Qt::DisplayRole && column == 2) {
        return client->fields["client_update"];
    }

    if (role == roleRole) {
        return client->fields["client_role"];
    }

    return EntityModel::data(index, role);
}

Client *ClientModel::createEntity() const
{
    return new Client();
}

QVariant ClientModel::getName(QVariant clientId)
{
    Entity *client = findEntity(clientId.toInt());
    if (client == NULL) {
        return QVariant();
    }

    return client->fields["client_name"];
}
