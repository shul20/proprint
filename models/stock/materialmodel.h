#ifndef MATERIALMODEL_H
#define MATERIALMODEL_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/stock/material.h"

class MaterialModel : public EntityModel
{
private:
    QStringList header;
    int type;

public:
    explicit MaterialModel(QObject *parent);

    QVariant data(const QModelIndex &index, int role) const;
    Material *createEntity() const;
    void setStockType(int type);
    
signals:
    
public slots:
    
};

#endif // MATERIALMODEL_H
