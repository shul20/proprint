#include "sizeedit.h"
#include <QRegExpValidator>

SizeEdit::SizeEdit(QWidget *parent) :
    QLineEdit(parent)
{
    QRegExpValidator *validator = new QRegExpValidator(QRegExp(Size::sizeRx), this);
    setValidator(validator);
}

Size SizeEdit::getSize(){
    return Size(text());
}

