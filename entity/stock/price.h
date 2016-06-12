#ifndef PRICE_H
#define PRICE_H

#include <QStringList>
#include "entity/entity.h"
#include "utility/money.h"
class Price : public Entity
{
private:
    static const QString table, tableView;
    QString product, type, company;

public:
    static const char allQuery[], priceWithStock[], priceNoStock[];

    Price();
    Price(Price *other);

    void setFields(const QSqlRecord &record);
    bool update();

    QString getProduct() const;
    void setProduct(const QString &value);
    QString getType() const;
    void setType(const QString &value);
    QString getCompany() const;
    void setCompany(const QString &value);

    static Money getPrice(int companyId, int productId, int priceTypeId,
                          int process, const QString &stockName);
};

#endif // PRICE_H
