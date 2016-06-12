#ifndef MONEYEDIT_H
#define MONEYEDIT_H

#include <QLineEdit>
#include <QRegExpValidator>
#include "utility/money.h"

class MoneyEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MoneyEdit(QWidget *parent = 0);
    ~MoneyEdit();

    Money getMoney();
    void setMoney(const Money &money);
    void setCustomColor();
    void setCustomPriceColor();
    void setDefaultColor();

signals:
    
};

#endif // MONEYEDIT_H
