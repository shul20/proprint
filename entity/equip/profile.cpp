#include "profile.h"

const char Profile::allQuery[] = "select * from profile";
const char Profile::passQuery[] = "select * from profile where pass_id = :id";
const QString Profile::table("profile");

Profile::Profile():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Profile::Profile(Profile *other):Entity(other)
{}


bool Profile::update()
{
    if (!fields["profile_name"].isValid()) {
        return false;
    }

    return Entity::update();
}
