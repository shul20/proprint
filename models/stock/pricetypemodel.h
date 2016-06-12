#ifndef PRICETYPEMODEL_H
#define PRICETYPEMODEL_H

#include "models/entitymodel.h"
#include "entity/stock/pricetype.h"

class PriceTypeModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit PriceTypeModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    PriceType *createEntity() const;
    
};

#endif // PRICETYPEMODEL_H
