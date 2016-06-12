#include "pay.h"

const QString Pay::table("pay");
const char Pay::allQuery[] = "select * from pay;";

const QStringList Pay::types =
        QStringList() << QString::fromUtf8("заказ")
                      << QString::fromUtf8("материал")
                      << QString::fromUtf8("склад")
                      << QString::fromUtf8("другое");

const QStringList Pay::credits =
        QStringList() << QString::fromUtf8("кредит")
                      << QString::fromUtf8("дебет");

const QStringList Pay::deposits =
        QStringList() << QString::fromUtf8("нет долга")
                      << QString::fromUtf8("создает долг")
                      << QString::fromUtf8("изменяет долг")
                      << QString::fromUtf8("списывает долг");

Pay::Pay():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Pay::Pay(Pay *other):Entity(other)
{
}
