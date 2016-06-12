#ifndef VARMODEL_H
#define VARMODEL_H

#include <QStringList>

#include "entity/client.h"
#include "entitymodel.h"
#include "entity/var.h"
#include "utility/money.h"

class VarModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;
    bool isRefresh;

public:
    explicit VarModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Var *createEntity() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void resetModel();
    void setIsRefresh(bool check);
    void refresh();

    QVariant getVar(int varId);
    void setVar(int varId, int value);


public slots:
    
};

#endif // VARMODEL_H
