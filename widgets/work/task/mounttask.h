#ifndef MOUNTTASK_H
#define MOUNTTASK_H

#include "widgets/work/task/taskwidget.h"

namespace Ui {
class MountTask;
}

class MountTask : public TaskWidget
{
    Q_OBJECT
    
public:
    explicit MountTask(QWidget *parent = 0);
    ~MountTask();

    void clear();
    bool form2Entity(Entity *task);
    void entity2Form(Entity *task);

private slots:
    void on_checkGroup_toggled(bool check);
    
private:
    Ui::MountTask *ui;
    Size size;

    void calculateSize();
    void calculateTotal();

    void initWidget(Stock *stock);
};

#endif // MOUNTTASK_H
