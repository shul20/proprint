#ifndef PRICEEDIT_H
#define PRICEEDIT_H

#include <QLabel>
#include "moneyedit.h"
#include "models/pricemodel.h"

class PriceEdit : public MoneyEdit
{
    Q_OBJECT
private:
    QString defaultStyle;


public:
    explicit PriceEdit(QWidget *parent = 0);
    void setMoney(const Money &money);
signals:
    
public slots:
    
};

#endif // PRICEEDIT_H
