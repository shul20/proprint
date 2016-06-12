#include "equipment.h"

const QStringList Equipment::statuses = QStringList()
                    << QString::fromUtf8("Рабочий")
                    << QString::fromUtf8("Законсервирован")
                    << QString::fromUtf8("Ремонт");

const QList<QColor> Equipment::colors = QList<QColor>()
                    << QColor("#FFFFFF")
                    << QColor("#CCC")
                    << QColor("#FFB2B2");


const char Equipment::allQuery[] = "select * from equipment";
const QString Equipment::table("equipment");

Equipment::Equipment(): Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Equipment::Equipment(Equipment *other): Entity(other)
{}

Equipment::~Equipment()
{}
