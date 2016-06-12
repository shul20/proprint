#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include "entity/entity.h"

class Product : public Entity
{
private:
    QString processesStr;
    static const QString table;

public:
    Product();
    Product(Product *other);
    ~Product();

    void setFields(const QSqlRecord &record);

    void setProcesses(int processes);
    const QString &getProcessesStr() const;
    void setProcessesStr();

    static const char allQuery[];
    static const char stockTypeQuery[];
    static const char allProcesses[];

};

#endif // PRODUCT_H
