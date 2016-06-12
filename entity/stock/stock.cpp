#include "stock.h"

const QString Stock::table("stock");
const QString Stock::tableView("stock_view");

const char Stock::allQuery[] = "select * from stock_view";
const char Stock::byTypeQuery[] = "select * from stock_view where stock_type = :type";
const char Stock::byProductQuery[] = "select * from stock_view where product_id = :id";
const char Stock::nameQuery[] = "select distinct stock_name from stock_view where product_id = :id;";

const QStringList Stock::stocks = QStringList()
        << QString::fromUtf8("Рулонный")
        << QString::fromUtf8("Листовой")
        << QString::fromUtf8("Штучный")
        << QString::fromUtf8("Не складируется");

Stock::Stock(): Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Stock::Stock(Stock *other): Entity(other), productName(other->productName)
{}

void Stock::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);
    productName = fields["product_name"].toString();
}


bool Stock::update()
{
    fields.remove("product_name");
    return Entity::update();
}


const QString &Stock::getProductName()
{
    return productName;
}

void Stock::setProductName(const QString &name)
{
    productName = name;
}
