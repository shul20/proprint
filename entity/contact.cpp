#include "contact.h"
#include "models/contactmodel.h"
#include "models/modelmanager.h"

#include <QDebug>

const QString Contact::table("contact");
const char Contact::allQuery[] = "select * from contact";
const char Contact::allByCompanyQuery[] = "select * from contact where company_id = :id";
const char Contact::allByCompanyTypeQuery[] =
        "select * from contact as c "
        "inner join company as co on co.company_type = :type "
        "and c.company_id = co.company_id;";


Contact::Contact():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Contact::Contact(Contact *other): Entity(other)
{}

bool Contact::update()
{
    int companyId = fields["company_id"].toInt();
    if(companyId == 0) {
        return true;
    }
    return Entity::update();
}

bool Contact::updateDebt(int contactId, int total)
{
    ContactModel *contactModel = ModelManager::getInstance()->getContactModel();
    Entity *contact = contactModel->findEntity(contactId);
    if (contact == NULL) {
        return false;
    }
    return contact->increment("contact_debt", total);
}

int Contact::getCompanyId(int contactId)
{
    QSqlQuery query;

    query.prepare(QString("select company_id from contact where contact_id = :id"));
    query.bindValue(":id", contactId);

    if (!query.exec()) {
        return 0;
    }
    query.next();
    return query.record().value(0).toInt();
}
