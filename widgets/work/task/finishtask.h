#ifndef FINISHTASK_H
#define FINISHTASK_H

#include "widgets/work/task/taskwidget.h"

namespace Ui {
class FinishTask;
}

class FinishTask : public TaskWidget
{
    Q_OBJECT
    
public:
    explicit FinishTask(QWidget *parent = 0);
    ~FinishTask();

    void clear();
    bool form2Entity(Entity *task);
    void entity2Form(Entity *task);

private slots:
    void on_checkGroup_toggled(bool check);
    void on_left_toggled(bool checked);
    void on_right_toggled(bool checked);
    void on_top_toggled(bool checked);
    void on_down_toggled(bool checked);
    
private:
    Ui::FinishTask *ui;

    void calculateSize();
    void calculateTotal();
};

#endif // FINISHTASK_H
