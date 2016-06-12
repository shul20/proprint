#include "pricetype.h"

const QString PriceType::table("pricetype");
const char PriceType::allQuery[] = "select * from pricetype;";

PriceType::PriceType():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

PriceType::PriceType(PriceType *other):Entity(other)
{
}
