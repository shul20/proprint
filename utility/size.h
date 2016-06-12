#ifndef SIZE_H
#define SIZE_H

#include <QString>
#include <QVariant>

class Size
{
public:
    int value;
    static const QString sizeRx;

    Size();
    Size(int widthMM, int heightMM);
    Size(QVariant value);
    Size(int value);
    Size(const QString &sizeString);

    Size operator*(int factor);
    bool operator ==(int compare);
    bool operator !=(int compare);
    void operator=(QVariant other);
    void operator=(int other);
    void operator=(Size other);

    bool operator>(Size other);
    bool operator<(Size other);

    Size operator+(Size other);
    Size operator-(Size other);
    void operator+=(Size other);
    void operator-=(Size other);

    QString toString();
    int getLengthM(int width);

};

#endif // SIZE_H
