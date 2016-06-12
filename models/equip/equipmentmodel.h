#ifndef EQUIPMENTMODEL_H
#define EQUIPMENTMODEL_H

#include <QStringList>

#include "entity/equip/equipment.h"
#include "models/entitymodel.h"


class EquipmentModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    EquipmentModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Equipment *createEntity() const;

    static const int processRole;
};

#endif // EQUIPMENTMODEL_H
