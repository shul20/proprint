#ifndef WORKMODEL_H
#define WORKMODEL_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/order/work.h"
#include "utility/size.h"

class WorkModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList headerForCreate, headerForTask;
    int tableMode;
public:
    explicit WorkModel(QObject *parent = 0);
    void setMode(int mode);
    QVariant data(const QModelIndex &index, int role) const;
    Work *createEntity() const;

    static const enum {CREATE = 0,  PROCESS = 1} modesEnum;
    void reset();

};

#endif // WORKMODEL_H
