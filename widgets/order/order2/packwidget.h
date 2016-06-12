#ifndef PACKWIDGET_H
#define PACKWIDGET_H

#include "widgets/order/order2/order2iwidget.h"

namespace Ui {
class PackWidget;
}

class PackWidget : public Order2IWidget
{
    Q_OBJECT
    
public:
    explicit PackWidget(QWidget *parent = 0);
    ~PackWidget();
    void clear();
    void entity2Form(Entity *order);
    bool form2Entity(Entity *order);
    void setReadOnly();
    
private:
    Ui::PackWidget *ui;
    int packType;

private slots:
    void on_packInfo_textChanged();
    void on_checkRoll_toggled(bool);
    void on_checkTube_toggled(bool);
    void on_checkOther_toggled(bool);
};

#endif // PACKWIDGET_H
