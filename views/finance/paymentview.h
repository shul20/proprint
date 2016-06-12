#ifndef PAYMENTVIEW_H
#define PAYMENTVIEW_H

#include <QWidget>
#include <QMenu>
#include <QDate>
#include "models/finance/paymentmodel.h"
#include "models/finance/accountmodel.h"

namespace Ui {
class PaymentView;
}

class PaymentView : public QWidget
{
    Q_OBJECT
    
public:
    explicit PaymentView(QWidget *parent = 0);
    ~PaymentView();
    
private:
    Ui::PaymentView *ui;
    PaymentModel *model;
    AccountModel *accountModel;
    QMenu *menu;

    void appendMenu();
    void refresh(QDate from, QDate till);

private slots:
    void on_table_customContextMenuRequested(const QPoint &pos);
    void on_dateFrom_dateChanged(const QDate &date);
    void on_dateTill_dateChanged(const QDate &date);
    void addPayment();
    void cacheChanged();
    void depositClose();
};

#endif // PAYMENTVIEW_H
