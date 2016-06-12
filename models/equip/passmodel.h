#ifndef PASSMODEL_H
#define PASSMODEL_H

#include "models/entitymodel.h"
#include "entity/equip/pass.h"

class PassModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit PassModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Pass *createEntity() const;

    static int modeRole;
};

#endif // PASSMODEL_H
