#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QToolButton>
#include "widgets/work/task/taskwidget.h"

namespace Ui {
class Tab;
}

class Tab : public QWidget
{
    Q_OBJECT
    
private:
    Ui::Tab *ui;
    QMap<QToolButton *, QWidget *> map;
    QWidget *currentOpen;

public:


    explicit Tab(QWidget *parent = 0);
    ~Tab();
    void setData(QList<QWidget *> &data);

public slots:
    void pressButton();
    void setCheck(bool check);
    void nameChanged();
    

};

#endif // TAB_H
