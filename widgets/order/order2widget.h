#ifndef ORDER2WIDGET_H
#define ORDER2WIDGET_H

#include <QWidget>
#include <QFrame>
#include <QToolButton>
#include "entity/order/order.h"
#include "order2/order2iwidget.h"
#include "order2/processinfow.h"
#include "order2/rightswidget.h"

namespace Ui {
class Order2Widget;
}

class Order2Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Order2Widget(QWidget *parent = 0);
    ~Order2Widget();
    void setOrder(Entity *order, int mode, bool isCopy = false);

    bool form2Entity(Entity *order);
    void clear();
    void setReadOnly();
    void setReadOnlyAll();

    static const enum {CREATE = 0, PROCESS = 1} modesEnum;

private slots:

private:
    Ui::Order2Widget *ui;
    QList<Order2IWidget *> widgets;
    ProcessInfoW *process;
    RightsWidget *rightsWidget;

};

#endif // ORDER2WIDGET_H
