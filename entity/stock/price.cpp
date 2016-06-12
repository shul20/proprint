#include "price.h"
#include "models/varmodel.h"
#include "QSqlRecord"
#include "models/modelmanager.h"

const QString Price::table("price");
const QString Price::tableView("price_view");
const char Price::allQuery[] = "select * from price_view;";
const char Price::priceWithStock[] =
             ("select * from price where "
              "(company_id = :company or company_id is NULL) and "
              "product_id = :product and "
              "pricetype_id = :priceType and "
              "price_process = :process and "
              "price_stock = :stock order by company_id;");

const char Price::priceNoStock[] =
             ("select * from price where "
              "(company_id = :company or company_id is NULL) and "
              "product_id = :product and "
              "pricetype_id = :priceType and "
              "price_process = :process order by company_id;");


Price::Price():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Price::Price(Price *other):Entity(other),
    product(other->product), type(other->type), company(other->company)
{
}

void Price::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);

    product = fields["product_name"].toString();
    type = fields["pricetype_name"].toString();
    company = fields["name_company"].toString();
}

bool Price::update()
{
    fields.remove("product_name");
    fields.remove("pricetype_name");
    fields.remove("name_company");

    return Entity::update();
}

QString Price::getCompany() const
{
    return company;
}

void Price::setCompany(const QString &value)
{
    company = value;
}

Money Price::getPrice(int companyId, int productId, int priceTypeId,
                      int process, const QString &stockName)
{
        QSqlQuery query;
        Money money;

        if (stockName.length() == 0) {
            query.prepare(priceNoStock);
            query.bindValue(":stock", QVariant(QVariant::String)); //NULL
        } else {
            query.prepare(priceWithStock);
            query.bindValue(":stock", stockName);
        }

        query.bindValue(":company", companyId);
        query.bindValue(":product", productId);
        query.bindValue(":priceType", priceTypeId);
        query.bindValue(":process", process);

        if (!query.exec() or !query.next()) {
            return money;
        }

        VarModel *var = ModelManager::getInstance()->getVarModel();
        Money rate(var->getVar(Var::RATE));
        money = Money (query.record().value("price_total")) * rate.cents;
        if (query.size() > 1) {
            money.isCustom = true;
        }

        return money;
}

QString Price::getType() const
{
    return type;
}

void Price::setType(const QString &value)
{
    type = value;
}

QString Price::getProduct() const
{
    return product;
}

void Price::setProduct(const QString &value)
{
    product = value;
}
