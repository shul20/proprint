#ifndef PAYMENTMODEL_H
#define PAYMENTMODEL_H

#include <QStringList>

#include "entity/finance/payment.h"
#include "models/entitymodel.h"

class PaymentModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit PaymentModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Payment *createEntity() const;
};

#endif // PAYMENTMODEL_H
