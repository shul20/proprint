#ifndef WOFF_H
#define WOFF_H

#include <QStringList>
#include "entity/entity.h"
#include "utility/size.h"

class Woff : public Entity
{
private:
    QVariant product, stockName, stockParam, client;
    int stockType, taskProcess;
    Size orderSize;
    Entity *stockEntity;
    static const QString table, tableView;

public:
    Woff();
    Woff(Woff *other);

    void setFields(const QSqlRecord &record);
    bool update();

    static const char allByStock[];

    QVariant getProduct() const;
    void setProduct(const QVariant &value);

    QVariant getStock() const;
    void setStock(const QVariant &value);

    Entity *getStockEntity() const;
    void setStockEntity(Entity *entity);

    QVariant getStockParam() const;
    void setStockParam(const QVariant &value);

    QVariant getClient() const;
    void setClient(const QVariant &value);

    int getStockType() const;
    void setStockType(int value);

    int getTaskProcess() const;
    void setTaskProcess(int value);

    Size getOrderSize() const;
    void setOrderSize(const Size &value);

    static Woff *getWoff(int taskId);
};

#endif // WOFF_H
