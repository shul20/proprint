#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QWidget>
#include <QMenu>
#include "entity/entity.h"
#include "widgets/work/workwidget.h"
#include "models/order/workmodel.h"

namespace Ui {
class PrintWidget;
}

class PrintWidget : public WorkWidget
{
    Q_OBJECT
    
public:
    explicit PrintWidget(QWidget *parent = 0);
    ~PrintWidget();
    void clear();
    bool update(int orderId);
    ProductWidget *getProductW();
    Entity *getEquip();
    void setOrder(int orderId, bool isCopy = false);
    
private slots:
    void on_clearButton_clicked();
    void addWork();
    void delWork();
    void modifyWork(QModelIndex index);
    void editWork();
    void refreshWorks();

    void totalChanged();

    void on_table_customContextMenuRequested(const QPoint &pos);

private:
    Ui::PrintWidget *ui;
    QMenu *menu;

    void setupMenu();
    bool updateTask(int workId);

};

#endif // PRINTWIDGET_H
