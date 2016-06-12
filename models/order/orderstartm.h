#ifndef ORDERSTARTM_H
#define ORDERSTARTM_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/entity.h"

class OrderStartM : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit OrderStartM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Entity *createEntity() const;

    static const int orderIdRole;
    
};

#endif // ORDERSTARTM_H
