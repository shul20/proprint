#include "company.h"
#include "mainwindow.h"
#include "models/modelmanager.h"
#include <QString>

Company::Company():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Company::Company(Company *other): Entity(other)
{}

const QString &Company::getTypeName()
{
    int type = fields["company_type"].toInt();
    return Company::companies[type];
}

bool Company::updateDebt(int companyId, int total)
{
    CompanyModel *model = ModelManager::getInstance()->getCompanyModel();
    Entity *company = model->findEntity(companyId);
    if (company == NULL) {
        return false;
    }

    return company->increment("company_debt", total);
}

const QString Company::table("company");
const char Company::allQuery[] = "select * from company";
const char Company::typeQuery[] = "select * from company where company_type = :type";
const char Company::fuzzyQuery[] = "select * from company "
                                   "where similarity(company_name, :name) > 0.3;";


const QStringList Company::companies = QStringList()
        << QString::fromUtf8("Заказчик")
        << QString::fromUtf8("Поставщик")
        << QString::fromUtf8("Субподрядчик");
