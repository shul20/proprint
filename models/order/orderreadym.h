#ifndef ORDERREADYM_H
#define ORDERREADYM_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/entity.h"

class OrderReadyM : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit OrderReadyM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Entity *createEntity() const;

    
};

#endif // ORDERREADYM_H
