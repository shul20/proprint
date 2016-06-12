#include "product.h"
#include "utility/process.h"

const QString Product::table("product");
const char Product::allQuery[] = "select * from product";
const char Product::stockTypeQuery[] = "select * from product where product_stock = :id";

Product::Product():Entity(){
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Product::Product(Product *other):Entity(other),
    processesStr(other->processesStr)
{}

Product::~Product()
{}

void Product::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);
    setProcessesStr();
}

void Product::setProcesses(int processes)
{
    fields["product_processes"] = processes;
    setProcessesStr();
}

const QString &Product::getProcessesStr() const
{
    return processesStr;
}

void Product::setProcessesStr()
{
    Process *process = Process::getInstance();
    int processes = fields["product_processes"].toInt();
    processesStr = process->getProcessStr(processes);
}
