#include "woff.h"

const QString Woff::table("woff");
const QString Woff::tableView("woff_view");

const char Woff::allByStock[] =
        "select * from woff_view where stock_type = :stock and "
        "(date(woff_date) between :from and :till) order by woff_date desc;";

Woff::Woff():Entity(), stockEntity(NULL)
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Woff::Woff(Woff *other):Entity(other)
{
    product = other->product;
    stockName = other->stockName;
    stockEntity = other->stockEntity;
    stockParam = other->stockParam;
    stockType = other->stockType;
    taskProcess = other->taskProcess;
    orderSize = other->orderSize;
    client = other->client;
}

void Woff::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);

    product = fields["product_name"];
    stockName = fields["stock_name"];
    stockParam = fields["stock_param"];
    stockType = fields["stock_type"].toInt();
    taskProcess = fields["task_process"].toInt();
    orderSize = fields["ordr_size"];
    client = fields["client_name"];
}

bool Woff::update()
{
    fields.remove("product_name");
    fields.remove("stock_name");
    fields.remove("stock_param");
    fields.remove("stock_type");
    fields.remove("task_process");
    fields.remove("ordr_size");
    fields.remove("client_name");

    return Entity::update();
}

Size Woff::getOrderSize() const
{
    return orderSize;
}

void Woff::setOrderSize(const Size &value)
{
    orderSize = value;
}

Woff *Woff::getWoff(int taskId)
{
    QSqlQuery query;
    Woff *woff = new Woff();

    query.prepare("select * from woff where task_id = :id limit 1");
    query.bindValue(":id", taskId);

    if (!query.exec() or !query.next()) {
        return NULL;
    }
    woff->setFields(query.record());

    return woff;
}

int Woff::getTaskProcess() const
{
    return taskProcess;
}

void Woff::setTaskProcess(int value)
{
    taskProcess = value;
}

int Woff::getStockType() const
{
    return stockType;
}

void Woff::setStockType(int value)
{
    stockType = value;
}

QVariant Woff::getStockParam() const
{
    return stockParam;
}

void Woff::setStockParam(const QVariant &value)
{
    stockParam = value;
}

QVariant Woff::getClient() const
{
    return client;
}

void Woff::setClient(const QVariant &value)
{
    client = value;
}

QVariant Woff::getStock() const
{
    return stockName;
}

Entity *Woff::getStockEntity() const
{
    return stockEntity;
}

void Woff::setStock(const QVariant &value)
{
    stockName = value;
}

void Woff::setStockEntity(Entity *entity)
{
    stockEntity = entity;
}

QVariant Woff::getProduct() const
{
    return product;
}

void Woff::setProduct(const QVariant &value)
{
    product = value;
}
