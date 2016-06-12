#ifndef PRINTTASK_H
#define PRINTTASK_H

#include "widgets/work/task/taskwidget.h"

namespace Ui {
class PrintTask;
}

class PrintTask : public TaskWidget
{
    Q_OBJECT
    
public:
    explicit PrintTask(QWidget *parent = 0);
    ~PrintTask();
    ProductWidget *getProductW();
    Entity *getEquip();
    int getPrintType();
    void clear();
    bool form2Entity(Entity *task);
    void entity2Form(Entity *task);
    void showEvent(QShowEvent *);


private slots:
    void on_printType_currentIndexChanged(int index);

    void on_pathButton_clicked();

    void on_priceForOne_textChanged(const QString &value);

    void on_over_textChanged(const QString &value);

private:
    Ui::PrintTask *ui;
    int sideAmount;
    Money priceForOne, priceOver;
    Size size;

    void calculateSize();
    void calculateTotal();

    void initWidget(Stock *stock);
};

#endif // PRINTTASK_H
