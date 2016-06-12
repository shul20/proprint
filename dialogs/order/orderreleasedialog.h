#ifndef ORDERRELEASEDIALOG_H
#define ORDERRELEASEDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include "entity/entity.h"

namespace Ui {
class OrderReleaseDialog;
}

class OrderReleaseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OrderReleaseDialog(QWidget *parent = 0);
    ~OrderReleaseDialog();
    void setModel(QAbstractItemModel *, Entity * view);
    
private slots:
    void on_no_clicked();

    void on_invoice_clicked();

    void on_yes_clicked();

private:
    Ui::OrderReleaseDialog *ui;
    Entity *view, *order;
};

#endif // ORDERRELEASEDIALOG_H
