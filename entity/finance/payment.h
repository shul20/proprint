#ifndef PAYMENT_H
#define PAYMENT_H

#include "entity/entity.h"

class Payment : public Entity
{
private:
    static const QString table, tableView;
    QVariant clientName, payName, payColor;
    int payType, payDebt, srcSign, dstSign;

public:
    Payment();
    Payment(Payment *other);
    void setFields(const QSqlRecord &record);
    bool update();

    static const char allByDate[], allBySource[], allByParent[];

    void setClient(const QVariant &name);
    QVariant &getClient();

    void setPayName(const QVariant &name);
    QVariant &getPayName();

    void setColor(const QVariant &color);
    QVariant &getColor();

    void setPayType(int type);
    int getPayType();

    void setPayDebt(int debt);
    int getPayDebt();

    void setSrcSign(int sign);
    int getSrcSign();

    void setDstSign(int sign);
    int getDstSign();

};

#endif // PAYMENT_H
