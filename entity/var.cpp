#include "var.h"

const QString Var::table("var");
const char Var::allQuery[] = "select * from var order by var_id;";

const QStringList Var::vars = QStringList()
                          << QString::fromUtf8("Курс")
                          << QString::fromUtf8("Норма списания")
                          << QString::fromUtf8("Обновление заказов")
                          << QString::fromUtf8("Обновление задач")
                          << QString::fromUtf8("Обновление параметров")
                          << QString::fromUtf8("Версия клиента");

Var::Var(): Entity()
{
    tableName = &table;
    tableView = &table;
}

Var::Var(Var *other): Entity(other)
{
}

