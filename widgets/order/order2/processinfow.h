#ifndef PROCESSINFOW_H
#define PROCESSINFOW_H

#include "widgets/order/order2/order2iwidget.h"
#include <QDateTime>

namespace Ui {
class ProcessInfoW;
}

class ProcessInfoW : public Order2IWidget
{
    Q_OBJECT
    
public:
    explicit ProcessInfoW(QWidget *parent = 0);
    ~ProcessInfoW();
    void clear();
    void entity2Form(Entity *order);
    bool form2Entity(Entity *order);
    void setReadOnly();
    void setWidgetReadOnly();
    
private:
    Ui::ProcessInfoW *ui;
    QDateTime start, predict, end;
    QString whoStart, whoEnd;

    void showData();

private slots:
    void on_log_textChanged();
};

#endif // PROCESSINFOW_H
