#ifndef STOCKMODEL_H
#define STOCKMODEL_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/stock/stock.h"

class StockModel : public EntityModel
{
private:
    static const QStringList header;

public:
    explicit StockModel(QObject *parent);

    QVariant data(const QModelIndex &index, int role) const;
    Stock *createEntity() const;

    static const enum {PRODUCT = 0, NAME = 1, PARAMETR = 2,
                AMOUNT = 3, STOCK = 4} fieldsEnum;

    static const int paramRole, amountRole, productIdRole,
                     stockTypeRole;
    
};

#endif // STOCKMODEL_H
