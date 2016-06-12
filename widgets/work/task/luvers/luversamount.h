#ifndef LUVERSAMOUNT_H
#define LUVERSAMOUNT_H

#include <QWidget>

namespace Ui {
class LuversAmount;
}

class LuversAmount : public QWidget
{
    Q_OBJECT
private:
    Ui::LuversAmount *ui;
    qlonglong length;
    int items, value;

    void calculateAmount();
    
public:
    int amount;

    explicit LuversAmount(QWidget *parent = 0);
    ~LuversAmount();
    void changeEvent(QEvent *event);

    void lengthChanged(qlonglong length, int items);

    void setState(int value, int state);
    int getState();

    int getValue();

private slots:

    void on_value_valueChanged(int value);

    void on_amount_toggled(bool checked);

    void on_step_toggled(bool checked);

signals:
    void amountChanged();
};

#endif // LUVERSAMOUNT_H
