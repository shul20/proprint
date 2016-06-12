#ifndef LAMINATTASK_H
#define LAMINATTASK_H

#include "widgets/work/task/taskwidget.h"

namespace Ui {
class LaminatTask;
}

class LaminatTask : public TaskWidget
{
    Q_OBJECT
    
public:
    explicit LaminatTask(QWidget *parent = 0);
    ~LaminatTask();

    Entity *getStock();
    Entity *getEquip();
    void clear();
    bool form2Entity(Entity *task);
    void entity2Form(Entity *task);
    void showEvent(QShowEvent *);
    
private slots:
    void on_checkGroup_toggled(bool check);

private:
    Ui::LaminatTask *ui;
    Size size;

    void calculateSize();
    void calculateTotal();

    void initWidget(Stock *stock);
};

#endif // LAMINATTASK_H
