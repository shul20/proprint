#ifndef INFOTASK_H
#define INFOTASK_H

#include "widgets/work/task/taskwidget.h"

namespace Ui {
class InfoTask;
}

class InfoTask : public TaskWidget
{
    Q_OBJECT
    
public:
    explicit InfoTask(QWidget *parent, int processId, bool isTotal = true);
    ~InfoTask();

    void clear();
    bool form2Entity(Entity *task);
    void entity2Form(Entity *task);

private slots:
    void on_checkGroup_toggled(bool check);

private:
    Ui::InfoTask *ui;

    void calculateTotal();
    void calculateSize();
};

#endif // INFOTASK_H
