#ifndef TASKSPWIDGET_H
#define TASKSPWIDGET_H

#include <QWidget>
#include <QAbstractItemModel>
#include "widgets/order/infowidget.h"
namespace Ui {
class TasksPWidget;
}

class TasksPWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TasksPWidget(QWidget *parent = 0);
    ~TasksPWidget();
    void setup(QAbstractTableModel *model, const QString &label, InfoWidget *info);

private:
    Ui::TasksPWidget *ui;

private slots:
    void editOrder(QModelIndex index);
};

#endif // TASKSPWIDGET_H
