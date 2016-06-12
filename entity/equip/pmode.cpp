#include "pmode.h"

const char Pmode::allQuery[] = "select * from pmode";
const char Pmode::equipQuery[] = "select * from pmode where equipment_id = :id";
const QString Pmode::table("pmode");

Pmode::Pmode(): Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Pmode::Pmode(Pmode *other): Entity(other)
{}

bool Pmode::update()
{
    int x = fields["pmode_x"].toInt();
    int y = fields["pmode_y"].toInt();

    if ((x == 0) || (y == 0)) {
        return false;
    }

    return Entity::update();
}
