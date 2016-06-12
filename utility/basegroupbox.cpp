#include "basegroupbox.h"
#include <QDebug>

BaseGroupBox::BaseGroupBox(QWidget *parent) :
    QGroupBox(parent), isBlock(false)
{
}

void BaseGroupBox::mousePressEvent(QMouseEvent *event){
    return;
}

void BaseGroupBox::keyPressEvent(QKeyEvent *e){
    return;
}

void BaseGroupBox::block(bool isBlock){
    this->isBlock = isBlock;
}


