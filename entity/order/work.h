#ifndef WORK_H
#define WORK_H

#include <QStringList>
#include "entity/entity.h"

class Work : public Entity
{
private:
    static const QString table, tableView;
    QVariant product, stock;
    int orderId;

public:
    Work();
    Work(Work *other);

    void setFields(const QSqlRecord &record);
    bool update();

    const QVariant &getProductName() const;
    void setProductName(const QVariant &name);

    const QVariant &getStockName() const;
    void setStockName(const QVariant &name);

    static const char allQuery[];
};

#endif // WORK_H
