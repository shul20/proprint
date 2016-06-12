#include "work.h"

const QString Work::table("work");
const QString Work::tableView("work_view");

const char Work::allQuery[] = "select * from work_view where ordr_id = :id";

Work::Work():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Work::Work(Work *other):Entity(other)
{
    product = other->product;
    stock = other->stock;
}

void Work::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);

    product = fields["product_name"];
    stock = fields["stock_name"];

}

bool Work::update()
{
    fields.remove("product_name");
    fields.remove("stock_name");

    return Entity::update();
}

const QVariant &Work::getProductName() const
{
    return product;
}

void Work::setProductName(const QVariant &name)
{
    product = name;
}

const QVariant &Work::getStockName() const
{
    return stock;
}

void Work::setStockName(const QVariant &name)
{
    stock = name;
}
