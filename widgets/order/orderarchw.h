#ifndef ORDERARCHW_H
#define ORDERARCHW_H

#include <QWidget>
#include <QMenu>
#include "models/order/orderarchm.h"
#include "widgets/order/infowidget.h"

namespace Ui {
class OrderArchW;
}

class OrderArchW : public QWidget
{
    Q_OBJECT
    
public:
    explicit OrderArchW(QWidget *parent = 0);
    ~OrderArchW();
    void setup(InfoWidget *info);
    
private:
    Ui::OrderArchW *ui;
    QMenu *menu;
    OrderArchM *model;

    void appendMenu();
    Entity *currentEntity();
    void refresh(QDate from, QDate till);

private slots:
    void on_table_customContextMenuRequested(const QPoint &pos);
    void on_dateFrom_dateChanged(const QDate &date);
    void on_dateTill_dateChanged(const QDate &date);

    void openOrder();
    void openProcess();
    void repeatOrder();
    void paymentOrder();
    void on_searchlButton_clicked();
};

#endif // ORDERARCHW_H
