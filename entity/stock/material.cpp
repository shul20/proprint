#include "material.h"
#include "entity/company.h"

const QString Material::table("material");
const QString Material::tableView("material_view");

const char Material::allQuery[] = "select * from material_view "
        "where stock_type = :type "
        "and ((date(material_receipt) between :start and :end) "
        "or material_debt > 0) "
        "order by material_receipt desc";

Material::Material():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Material::Material(Material *other)
    :Entity(other), stock(other->stock), company(other->company),
      contact(other->contact), product(other->product),
      stockType(other->stockType), stockParam(other->stockParam),
      companyId(other->companyId)
{
}

void Material::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);
    stock = fields["stock_name"].toString();
    company = fields["company_name"].toString();
    companyId = fields["company_id"].toInt();
    contact = fields["contact_name"].toString();
    product = fields["product_name"].toString();
    stockType = fields["stock_type"].toInt();
    stockParam = fields["stock_param"].toInt();
}

bool Material::update()
{
    fields.remove("stock_name");
    fields.remove("company_name");
    fields.remove("company_id");
    fields.remove("contact_name");
    fields.remove("product_name");
    fields.remove("stock_type");
    fields.remove("stock_param");

    return Entity::update();
}

int Material::getStockType()
{
    return stockType;
}

void Material::setStockType(int value)
{
    stockType = value;
}


const QString &Material::getStockName()
{
    return stock;
}

void Material::setStockName(const QString &name)
{
    stock = name;
}

const QString Material::getCompanyName()
{
    return company;
}

void Material::setCompanyName(const QString &name)
{
    company = name;
}

int Material::getCompanyId()
{
    return companyId;
}

void Material::setCompanyId(int value)
{
    companyId = value;
}

const QString Material::getContactName()
{
    return contact;
}

void Material::setContactName(const QString &name)
{
    contact = name;
}

const QString &Material::getProductName()
{
    return product;
}

void Material::setProductName(const QString &name)
{
    product = name;
}

int Material::getParametr()
{
    return stockParam;
}

void Material::setParametr(int value)
{
    stockParam = value;
}

QPair<Money, Money> Material::getPriceRange(int stockId)
{
    QSqlQuery query;

    query.prepare(QString("select min(p.material_price) as minPrice, max(p.material_price) as maxPrice "
                          "from (select material_price from material "
                          "where stock_id = :id limit 100) as p;"));

    query.bindValue(":id", stockId);
    query.exec();
    query.next();

    const QSqlRecord r = query.record();
    return QPair<Money, Money>
            (Money(r.value("minPrice")), Money(r.value("maxPrice")));
}

int Material::getContactId(int materialId)
{
    QSqlQuery query;

    query.prepare(QString("select contact_id from material where material_id = :id"));
    query.bindValue(":id", materialId);

    if (!query.exec()) {
        return 0;
    }
    query.next();
    return query.record().value(0).toInt();
}
