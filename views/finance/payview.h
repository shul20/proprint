#ifndef PAYVIEW_H
#define PAYVIEW_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class PayView;
}

class PayView : public QWidget
{
    Q_OBJECT
    
public:
    explicit PayView(QWidget *parent = 0);
    ~PayView();
    
private:
    Ui::PayView *ui;

private slots:
    void addPay();
    void editPay();
    void editPay(QModelIndex index);
};

#endif // PAYVIEW_H
