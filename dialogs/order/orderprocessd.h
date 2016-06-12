#ifndef ORDERPROCESSD_H
#define ORDERPROCESSD_H

#include <QDialog>
#include "entity/order/order.h"
#include "widgets/workflow/workflowwidget.h"

namespace Ui {
class OrderProcessD;
}

class OrderProcessD : public QDialog
{
    Q_OBJECT
    
public:
    explicit OrderProcessD(QWidget *parent = 0);
    ~OrderProcessD();
    void setModel(QAbstractItemModel *model, Entity * view);
    
private slots:
    void on_exitButton_clicked();
    void update();
    void startOrder();
    void endOrder();

private:
    Ui::OrderProcessD *ui;
    WorkFlowWidget *flowWidget;
    Entity *order, *orderView;
    QAbstractItemModel *model;

    void setInfo();
    void setFlowWidget(Entity *order);
};

#endif // ORDERPROCESSD_H
