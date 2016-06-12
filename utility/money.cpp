#include "money.h"
#include <QDebug>

const QString Money::moneyRx("[0-9]{,7}[.][0-9]{2}");
const QStringList Money::paymentType = QStringList() << QString::fromUtf8("Безналичный")
                                                     << QString::fromUtf8("Наличный")
                                                     << QString::fromUtf8("Залог");

Money::Money(qlonglong cents):
    cents(cents), isCustom(false){}

Money::Money(int cents):
    cents((qlonglong)cents), isCustom(false){}

Money::Money(QVariant cents):
    cents(cents.toLongLong()), isCustom(false){}

Money::Money(QString moneyString): isCustom(false){
    QStringList parts = moneyString.split('.');
    qlonglong result = parts[0].toLongLong() * 100;

    if (parts.length() != 1) {
        qlonglong remain;

        remain = parts[1].toLongLong();
        if (parts[1].length() == 2) {
            result += remain;
        } else {
            result += remain * 10;
        }
    }
    cents = result;
}

void Money::operator =(qlonglong cents){
    if (cents < 0) {
        throw NotAllowedValue();
    }
    this->cents = cents;
}

void Money::operator =(const Money &other){
    cents = other.cents;
    isCustom = other.isCustom;
}

bool Money::operator >(const Money &other) const{
    return cents > other.cents;
}

bool Money::operator >=(const Money &other) const{
    return cents >= other.cents;
}

bool Money::operator <(const Money &other) const{
return cents < other.cents;
}

bool Money::operator <=(const Money &other) const{
    return cents <= other.cents;
}

bool Money::operator ==(const Money &other) const{
    return cents == other.cents;
}

bool Money::operator ==(qlonglong cents) const{
    return this->cents == cents;
}

bool Money::operator !=(const Money &other) const{
    return cents != other.cents;
}

bool Money::operator !=(qlonglong cents) const{
    return this->cents != cents;
}

Money Money::operator *(qlonglong factor) const{
    qlonglong x = cents * factor;
    qlonglong result  = x/100;
    if ( x%100 >= 50 ){
        result += 1;
    }
    return  Money(result);
}

Money Money::operator *(Size size) const{
    return Money(cents) * size.value;
}

Money Money::operator /(qlonglong factor) const{
    qlonglong x = cents * 1000;
    qlonglong round = x/factor;
    qlonglong result = round/10;

    if (round%10 >= 5) {
        result += 1;
    }
    return Money(result);
}

Money Money::operator +(const Money &other) const{
    return Money(cents + other.cents);
}

Money Money::operator -(const Money &other) const{
    return Money(cents - other.cents);
}

void Money::operator -=(const Money &other) {
    cents -= other.cents;
}

QString Money::toString() const{
    if (cents < 0) {
        qlonglong centsAbs = -cents;
        return QString("-%1.%2").arg(centsAbs/100).arg(centsAbs%100, 2, 10, QChar('0'));
    }
    return QString("%1.%2").arg(cents/100).arg(cents%100, 2, 10, QChar('0'));
}
