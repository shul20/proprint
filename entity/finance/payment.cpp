#include "payment.h"
#include "utility/money.h"
#include "models/varmodel.h"
#include "models/modelmanager.h"
#include <QDateTime>

const QString Payment::table("payment");
const QString Payment::tableView("payment_view");

const char Payment::allByDate[] =
        "select * from payment_view where "
        "((date(payment_date) between :from and :till) or payment_debt != 0) and "
        ":client <= pay_permission "
        "order by payment_date desc";

const char Payment::allBySource[] =
        "select * from payment_view where payment_field_id = :id and "
        "pay_type = :type and :client <= pay_permission "
        "order by payment_date desc";

const char Payment::allByParent[] =
        "select * from payment_view where payment_parent = :id and "
        "pay_type = :type and :client <= pay_permission "
        "order by payment_date desc";

Payment::Payment():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Payment::Payment(Payment *other):Entity(other),
    clientName(other->clientName), payName(other->payName), payColor(other->payColor),
    payType(other->payType), payDebt(other->payDebt)
{}

void Payment::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);
    clientName = fields["client_name"];
    payName = fields["pay_name"];
    payColor = fields["pay_color"];
    payType = fields["pay_type"].toInt();
    payDebt = fields["pay_debt"].toInt();
    srcSign = fields["src_sign"].toInt();
    dstSign = fields["dst_sign"].toInt();
}

bool Payment::update()
{
    fields.remove("client_name");
    fields.remove("pay_name");
    fields.remove("pay_color");
    fields.remove("pay_type");
    fields.remove("pay_debt");
    fields.remove("src_sign");
    fields.remove("dst_sign");
    return Entity::update();
}


void Payment::setClient(const QVariant &name)
{
    clientName = name;
}

QVariant &Payment::getClient()
{
    return clientName;
}

void Payment::setPayName(const QVariant &name)
{
    payName = name;
}

QVariant &Payment::getPayName()
{
    return payName;
}

void Payment::setColor(const QVariant &color)
{
    payColor = color;
}

QVariant &Payment::getColor()
{
    return payColor;
}

void Payment::setPayType(int type)
{
    payType = type;
}

int Payment::getPayType()
{
    return payType;
}

void Payment::setPayDebt(int debt)
{
    payDebt = debt;
}

int Payment::getPayDebt()
{
    return payDebt;
}

void Payment::setSrcSign(int sign)
{
    srcSign = sign;
}

int Payment::getSrcSign()
{
    return srcSign;
}

void Payment::setDstSign(int sign)
{
    dstSign = sign;
}

int Payment::getDstSign()
{
    return dstSign;
}
