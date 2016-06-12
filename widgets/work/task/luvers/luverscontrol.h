#ifndef LUVERSCONTROL_H
#define LUVERSCONTROL_H

#include <QWidget>
#include "widgets/work/task/luvers/luversamount.h"

namespace Ui {
class LuversControl;
}

class LuversControl : public QWidget
{
    Q_OBJECT
private:
    Ui::LuversControl *ui;

    void toggled(LuversAmount *block, bool check);
    void resetCustomCheck();

public:
    explicit LuversControl(QWidget *parent = 0);
    ~LuversControl();
    void setData(int checks, qlonglong &values);
    void getData(int &checks, qlonglong &values);
    void clear();
    int getAmount();

public slots:
    void sizeChanged(int width, int height, int amount);
     void calculateAmount();

    void on_schemaBox_toggled(bool check);
    void on_perimetrBox_toggled(bool check);
    void on_leftBox_toggled(bool check);
    void on_upBox_toggled(bool check);
    void on_rightBox_toggled(bool check);
    void on_downBox_toggled(bool check);



signals:
    void amountChanged(int amount);
    
};

#endif // LUVERSCONTROL_H
