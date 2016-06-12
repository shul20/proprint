#include "account.h"

const QString Account::table("account");
const char Account::allQuery[] = "select * from account order by account_id;";

const QStringList Account::accounts = QStringList()
                          << QString::fromUtf8("Основной")
                          << QString::fromUtf8("Залоговый");

Account::Account():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Account::Account(Account *other):Entity(other)
{
}
