#ifndef MONEY_H
#define MONEY_H

#include <QString>
#include <QVariant>
#include <QStringList>
#include "size.h"

class NotAllowedValue{};

class Money{

public:
    qlonglong cents;
    bool isCustom;
    static const QString moneyRx;
    static const QStringList paymentType;

    Money(qlonglong cents = 0);
    Money(int cents);
    Money(QVariant cents);
    Money(QString moneyString);

    QString toString() const;

    void operator =(qlonglong cents);
    void operator =(const Money &other);

    bool operator >(const Money &other) const;
    bool operator >=(const Money &other) const;
    bool operator <(const Money &other) const;
    bool operator <=(const Money &other) const;

    bool operator ==(const Money &other) const;
    bool operator ==(qlonglong cents) const;
    bool operator !=(const Money &other) const;
    bool operator !=(qlonglong cents) const;

    Money operator *(qlonglong factor) const;
    Money operator *(Size factor) const;
    Money operator /(qlonglong factor) const;

    Money operator +(const Money &other) const;
    Money operator -(const Money &other) const;

    void operator -=(const Money &other);
};

#endif // MONEY_H
