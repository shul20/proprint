#include "companymodel.h"

const int CompanyModel::typeRole = PPLib::userRole + 1;

const QStringList CompanyModel::header =
        QStringList() << QString::fromUtf8("Название")
                      << QString::fromUtf8("Тип")
                      << QString::fromUtf8("Адрес")
                      << QString::fromUtf8("Телефон")
                      << QString::fromUtf8("Счет");

CompanyModel::CompanyModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}


QVariant CompanyModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    int row = index.row();
    if (row < 0) {
        return QVariant();
    }
    Company *company = static_cast<Company *>(getEntity(row));

    if ((column == 0) && ((role == Qt::DisplayRole) || role == (Qt::EditRole))) {
        return company->fields["company_name"];
    }

    if (role == Qt::DisplayRole && column == 1) {
        return company->getTypeName();
    }

    if (role == Qt::DisplayRole && column == 2) {
        return company->fields["company_address"];
    }

    if (role == Qt::DisplayRole && column == 3) {
        return company->fields["company_tel"];
    }

    if (role == Qt::DisplayRole && column == 4) {
        return company->fields["company_account"];
    }

    if (role == typeRole) {
        return company->fields["company_type"];
    }

    return EntityModel::data(index, role);
}

Company *CompanyModel::createEntity() const
{
    return new Company();
}
