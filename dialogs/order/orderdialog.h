#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QDialog>
#include "dialogs/entitydialog.h"
#include "models/companymodel.h"
#include "widgets/work/workwidget.h"
#include "entity/order/order.h"

namespace Ui {
class OrderDialog;
}

class OrderDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OrderDialog(QWidget *parent = 0);
    ~OrderDialog();
    void setModel(QAbstractItemModel *model, int mode,
                  Entity * view);
private slots:
    void on_saveOrderButton_clicked();

    void on_process_currentIndexChanged(int index);
    void worksChanged();
    void blockInput(bool check);

    void on_exitOrderButton_clicked();
    void setReadOnly();

private:
    Ui::OrderDialog *ui;
    WorkWidget *workWidget;
    Order *order;
    QAbstractItemModel *model;
    int mode;
    Entity * orderView;


    bool form2Entity(Entity *entity);
    void entity2Form(Entity *order, Entity *orderView);
    void clear();
};

#endif // ORDERDIALOG_H
