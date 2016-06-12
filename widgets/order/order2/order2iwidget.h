#ifndef ORDER2IWIDGET_H
#define ORDER2IWIDGET_H

#include <QWidget>
#include "entity/order/order.h"


class Order2IWidget : public QWidget
{
    Q_OBJECT
public:
    explicit Order2IWidget(QWidget *parent = 0);
    virtual void clear() = 0;
    virtual void entity2Form(Entity *order) = 0;
    virtual bool form2Entity(Entity *order) = 0;
    virtual void setReadOnly() = 0;
    void setMode(int value);

protected:
    void checkText(const QString &text);
    int mode;
    bool isReadOnly;
    
signals:
    void setCheck(bool);
    void nameChanged();

public slots:
    
};

#endif // ORDER2IWIDGET_H
