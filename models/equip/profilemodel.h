#ifndef PROFILEMODEL_H
#define PROFILEMODEL_H

#include "models/entitymodel.h"
#include "entity/equip/profile.h"

class ProfileModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    static int passRole;

    explicit ProfileModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Profile *createEntity() const;
};

#endif // PROFILEMODEL_H
