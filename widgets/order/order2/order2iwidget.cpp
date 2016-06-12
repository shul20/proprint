#include "order2iwidget.h"

Order2IWidget::Order2IWidget(QWidget *parent) :
    QWidget(parent), isReadOnly(false)
{
}

void Order2IWidget::setMode(int value)
{
    mode = value;
}

void Order2IWidget::checkText(const QString &text)
{
    if (text.length() == 0) {
        emit setCheck(false);
        return;
    }
    emit setCheck(true);
}
