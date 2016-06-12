#include "priceedit.h"
#include <QToolTip>


PriceEdit::PriceEdit(QWidget *parent) :
    MoneyEdit(parent)
{
    setReadOnly(true);
    setStyleSheet("background: rgba(255, 255, 255, 55)");
    defaultStyle = styleSheet();

}

void PriceEdit::setMoney(const Money &money){
    if (money.isCustom) {
        setStyleSheet("background: #FFA375");
        setToolTip(QString::fromUtf8("Персональный прайс!"));

    } else {
        setStyleSheet(defaultStyle);
        setToolTip("");
    }

    MoneyEdit::setMoney(money);
}

