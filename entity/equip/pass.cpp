#include "pass.h"

const char Pass::allQuery[] = "select * from pass";
const char Pass::modeQuery[] = "select * from pass where pmode_id = :id";
const QString Pass::table("pass");

Pass::Pass(): Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;

}

Pass::Pass(Pass *other): Entity(other)
{
}

bool Pass::update()
{
    int pass = fields["pass_name"].toInt();

    if (pass == 0) {
        return false;
    }

    return Entity::update();
}
