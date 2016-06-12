#ifndef PMODEMODEL_H
#define PMODEMODEL_H

#include "entity/equip/pmode.h"
#include "models/entitymodel.h"

class PmodeModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit PmodeModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex & index) const ;

    Pmode *createEntity() const;
    void setEquipment(int equipId);

    static int equipIdRole;
    
signals:
    
public slots:
    
};

#endif // PMODEMODEL_H
