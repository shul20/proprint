#ifndef LUVERSTASK_H
#define LUVERSTASK_H

#include "widgets/work/task/taskwidget.h"
#include "widgets/work/task/luvers/luversamount.h"

namespace Ui {
class LuversTask;
}

class LuversTask : public TaskWidget
{
    Q_OBJECT
private:
    Ui::LuversTask *ui;
    int amount;

    void calculateSize();
    void calculateTotal();

    void initWidget(Stock *stock);

public:
    explicit LuversTask(QWidget *parent = 0);
    ~LuversTask();

    void clear();
    bool form2Entity(Entity *task);
    void entity2Form(Entity *task);

public slots:
    void amountChanged(int amount);
    void on_checkGroup_toggled(bool check);
    
};

#endif // LUVERSTASK_H
