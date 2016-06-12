#ifndef MATERIAL_H
#define MATERIAL_H

#include "entity/entity.h"
#include "utility/money.h"

class Material : public Entity
{
private:
    QString stock, company, contact, product;
    int stockType, stockParam, companyId;
    static const QString table, tableView;

public:
    Material();
    Material(Material *other);

    void setFields(const QSqlRecord &record);
    bool update();

    int getStockType();
    void setStockType(int value);

    const QString &getStockName();
    void setStockName(const QString &name);

    const QString getCompanyName();
    void setCompanyName(const QString &name);

    int getCompanyId();
    void setCompanyId(int value);

    const QString getContactName();
    void setContactName(const QString &name);

    const QString &getProductName();
    void setProductName(const QString &name);

    int getParametr();
    void setParametr(int value);

    static int getContactId(int materialId);

    static const char allQuery[];
    static QPair<Money, Money> getPriceRange(int stockId);
};

#endif // MATERIAL_H
