#ifndef COMPANYMODEL_H
#define COMPANYMODEL_H

#include <QStringList>
#include "models/entitymodel.h"
#include "entity/company.h"
#include "dialogs/companydialog.h"

class CompanyModel : public EntityModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit CompanyModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Company *createEntity() const;

    static const int typeRole;
};

#endif // COMPANYMODEL_H
