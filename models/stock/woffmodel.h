#ifndef WOFFMODEL_H
#define WOFFMODEL_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/stock/woff.h"

class WoffModel : public EntityModel
{
    Q_OBJECT
private:
    QStringList header;
    int type;

public:
    explicit WoffModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Woff *createEntity() const;
    void setStockType(int type);
    
signals:
    
public slots:
    
};

#endif // WOFFMODEL_H
