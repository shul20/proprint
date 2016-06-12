#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QStringList>
#include <models/entitymodel.h>
#include "entity/contact.h"

class ContactModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit ContactModel(QObject *parent = 0);
    bool update(int companyId);
    QVariant data(const QModelIndex &index, int role) const;
    Contact *createEntity() const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    static const int companyIdRole;
    static bool update(QAbstractItemModel *model, int companyId);
};

#endif // CONTACTMODEL_H
