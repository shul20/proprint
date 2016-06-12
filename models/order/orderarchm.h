#ifndef ORDERARCHM_H
#define ORDERARCHM_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/entityview.h"

class OrderArchM : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit OrderArchM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    EntityView *createEntity() const;

};

#endif // ORDERARCHM_H
