#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include "models/entitymodel.h"
#include "entity/product.h"

class ProductModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit ProductModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Product *createEntity() const;

    static int processRole;
    static int stockRole;
};

#endif // PRODUCTMODEL_H
