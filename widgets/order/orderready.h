#ifndef ORDERREADY_H
#define ORDERREADY_H

#include <QWidget>
#include <QTimer>
#include "models/order/orderreadym.h"
#include "widgets/order/infowidget.h"

namespace Ui {
class OrderReady;
}

class OrderReady : public QWidget
{
    Q_OBJECT
    
public:
    explicit OrderReady(QWidget *parent = 0);
    ~OrderReady();
    void setup(InfoWidget *info);
    void hide();
    void show();

private:
    Ui::OrderReady *ui;
    OrderReadyM *model;
    QTimer *timer;
    QMenu *menu;

    void appendMenu();
    Entity *currentEntity();

private slots:
    void on_table_customContextMenuRequested(const QPoint &pos);
    void openOrder();
    void openProcess();
    void releaseOrder();
    void pauseOrder();
    void modifyOrder(QModelIndex);
    void refresh();
    bool event(QEvent *event);
};

#endif // ORDERREADY_H
