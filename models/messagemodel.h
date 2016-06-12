#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QStringList>
#include <models/entitymodel.h>
#include "entity/message.h"

class MessageModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit MessageModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Message *createEntity() const;

signals:
    
public slots:
    
};

#endif // MESSAGEMODEL_H
