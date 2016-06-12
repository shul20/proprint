#ifndef DELIVERYWIDGET_H
#define DELIVERYWIDGET_H

#include "widgets/order/order2/order2iwidget.h"

namespace Ui {
class DeliveryWidget;
}

class DeliveryWidget : public Order2IWidget
{
    Q_OBJECT
    
public:
    explicit DeliveryWidget(QWidget *parent = 0);
    ~DeliveryWidget();
    void clear();
    void entity2Form(Entity *order);
    bool form2Entity(Entity *order);
    void setReadOnly();
    
private:
    Ui::DeliveryWidget *ui;

private slots:
    void on_deliveryType_currentIndexChanged(int index);
    void on_deliveryInfo_textChanged();
};

#endif // DELIVERYWIDGET_H
