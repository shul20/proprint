#include "moneyedit.h"
#include <QDebug>

MoneyEdit::MoneyEdit(QWidget *parent) :
    QLineEdit(parent)
{
    QRegExpValidator *validator = new QRegExpValidator(QRegExp(Money::moneyRx), this);
    setValidator(validator);
}

MoneyEdit::~MoneyEdit(){
    delete validator();
}

Money MoneyEdit::getMoney(){
    return Money(text());
}

void MoneyEdit::setMoney(const Money &money){
    setText(money.toString());

    if (money.isCustom) {
        setCustomColor();
    } else {
        setDefaultColor();
    }
}

void MoneyEdit::setCustomColor(){
    QString objName = objectName();

    if (objName == QString("price")) {
        setStyleSheet("QLineEdit#price {background: #FFA375}");
        setToolTip(QString::fromUtf8("Персональный прайс!"));

    } else if (objName == QString("total")) {
        setStyleSheet("QLineEdit#total {background: #FF9191}");
        setToolTip(QString::fromUtf8("Цена не по прайсу!"));

    } else if (objName == QString("totalInput")) {
        setStyleSheet("QLineEdit#totalInput {background: #FF9191}");
        setToolTip(QString::fromUtf8("Цена не по прайсу!"));
    }
}

void MoneyEdit::setDefaultColor(){
    setToolTip("");
    QString objName = objectName();

    if (objName == QString("totalInput")) {
        setStyleSheet("QLineEdit#totalInput {background: #FFFFFF}");

    } else if ((objName == QString("total")) ||
               (objName == QString("price"))) {
        setStyleSheet("QLineEdit {background: rgba(255,255,255,55)}");
    }
}

