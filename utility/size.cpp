#include "size.h"
#include <QStringList>

const QString Size::sizeRx("[0-9]{,7}[.][0-9]{2}");

Size::Size(): value(0){}
Size::Size(QVariant value): value(value.toInt()){}

Size::Size(int value): value(value){}

Size::Size(const QString &sizeString){
    QStringList parts = sizeString.split('.');
    int result = parts[0].toLongLong() * 100;

    if (parts.length() != 1) {
        int remain;

        remain = parts[1].toLongLong();
        if (parts[1].length() == 2) {
            result += remain;
        } else {
            result += remain * 10;
        }
    }
    value = result;
}

Size Size::operator *(int factor){
    return Size(value * factor);
}

bool Size::operator ==(int compare){
    return value == compare;
}

bool Size::operator !=(int compare){
    return value != compare;
}

void Size::operator =(QVariant other){
    this->value = other.toInt();
}

void Size::operator =(int other){
    this->value = other;
}

void Size::operator =(Size other){
    this->value = other.value;
}

bool Size::operator >(Size other){
    return value > other.value;
}

bool Size::operator <(Size other){
    return value < other.value;
}

Size Size::operator +(Size other){
    return Size(value + other.value);
}

Size Size::operator -(Size other){
    return Size(value - other.value);
}

void Size::operator +=(Size other){
    this->value += other.value;
}

void Size::operator -=(Size other){
    this->value -= other.value;
}

Size::Size(int widthMM, int heightMM){
    qlonglong size =  ((qlonglong) widthMM  * (qlonglong) heightMM) / 100;
    value = (int) (size / 100);
    if ((size % 100) >= 50) {
        value += 1;
    }
}

QString Size::toString(){
    return QString::number((double)value/100);
}

int Size::getLengthM(int width)
{

    return  (int) ((qlonglong)value * 10) / (qlonglong)width;
}

