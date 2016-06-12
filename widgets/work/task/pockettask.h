#ifndef POCKETTASK_H
#define POCKETTASK_H

#include "widgets/work/task/taskwidget.h"

namespace Ui {
class PocketTask;
}

class PocketTask : public TaskWidget
{
    Q_OBJECT
    
public:
    explicit PocketTask(QWidget *parent = 0);
    ~PocketTask();

    void clear();
    bool form2Entity(Entity *task);
    void entity2Form(Entity *task);

private slots:
    void on_checkGroup_toggled(bool check);
    
private:
    Ui::PocketTask *ui;
    int isTop, isBottom;

    void calculateSize();
    void calculateTotal();

    void initWidget(Product *product);

private slots:
    void on_top_valueChanged(int value);
    void on_bottom_valueChanged(int value);
};

#endif // POCKETTASK_H
