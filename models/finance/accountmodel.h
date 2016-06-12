#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include "entity/finance/account.h"
#include "utility/money.h"

class AccountModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;
public:
    explicit AccountModel(QObject *parent = 0);
    Account *createEntity() const;

    QVariant data(const QModelIndex &index, int role) const;

signals:
    
public slots:
    
};

#endif // ACCOUNTMODEL_H
