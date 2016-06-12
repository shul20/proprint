#ifndef PAYMODEL_H
#define PAYMODEL_H

#include "entity/finance/pay.h"
#include "models/entitymodel.h"

class PayModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

    const QString &getCreditName(int sign) const;

public:
    explicit PayModel(QObject *parent = 0);
    Pay *createEntity() const;
    QVariant data(const QModelIndex &index, int role) const;

    static const int materialRole, debtMaterialRole, otherRole,
        debtOtherRole, orderRole, debtOrderRole, stockRole;
    
signals:
    
public slots:
    
};

#endif // PAYMODEL_H
