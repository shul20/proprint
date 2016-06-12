#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QStringList>

#include "entity/client.h"
#include "entitymodel.h"
#include "dialogs/clientdialog.h"

class ClientModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit ClientModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Client *createEntity() const;
    QVariant getName(QVariant clientId);
    Client *findClient(int id);

    static const int roleRole;
};

#endif // CLIENTMODEL_H
