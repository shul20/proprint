#ifndef PRICEMODEL_H
#define PRICEMODEL_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/stock/price.h"

class PriceModel : public EntityModel
{
    Q_OBJECT
private:
    static QStringList header;

public:
    explicit PriceModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Price *createEntity() const;

    static const int processRole;
    static const int companyRole;
    
signals:
    
public slots:
    
};

#endif // PRICEMODEL_H
