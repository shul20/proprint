#ifndef ORDERSTART_H
#define ORDERSTART_H

#include <QWidget>
#include <QTimer>
#include <QMenu>
#include "views/workflow.h"
#include "models/order/orderstartm.h"
#include "widgets/order/infowidget.h"

namespace Ui {
class OrderStart;
}

class OrderStart : public QWidget
{
    Q_OBJECT
    
public:
    explicit OrderStart(QWidget *parent = 0);
    ~OrderStart();
    void setup(WorkFlow *workFlow, InfoWidget *info);
    
private:
    Ui::OrderStart *ui;
    QString allowStatuses;
    WorkFlow *workFlow;
    QTimer *timer;
    QMenu *menu;
    OrderStartM *model;

    void appendMenu();
    void setPermission();
    Entity *currentEntity();
    void stopTimer();

private slots:
    void setStatus(QAction *action);
    void setPriority(QAction*action);
    void addOrder();
    void delOrder();
    void editOrder();
    void modifyOrder(QModelIndex index);
    void on_table_customContextMenuRequested(const QPoint &pos);
    void refresh();
    void paymentOrder();
    bool event(QEvent *event);
};

#endif // ORDERSTART_H
