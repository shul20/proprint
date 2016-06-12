#ifndef STOCK_H
#define STOCK_H

#include <QStringList>
#include "entity/entity.h"
#include "entity/product.h"

class Stock : public Entity
{
private:
    QString productName;
    static const QString table, tableView;

public:
    explicit Stock();
    Stock(Stock *other);

    void setFields(const QSqlRecord &record);
    bool update();
    
    const QString &getProductName();
    void setProductName(const QString &name);

    static const char allQuery[], nameQuery[], byProductQuery[], byTypeQuery[];
    static const QStringList stocks;
    static const enum {ROLL = 0, LIST = 1, ITEM = 2, NOSTOCK = 3} stockEnum;

};

#endif // STOCK_H
