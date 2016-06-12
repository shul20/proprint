#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>
#include "entity/product.h"
#include "models/processmodel.h"
#include "models/entitymodel.h"

namespace Ui {
class ProductDialog;
}

class ProductDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProductDialog(QWidget *parent);
    ~ProductDialog();
    void set(QAbstractItemModel *model, int mode, const QModelIndex &index);
    
private slots:
    void on_saveProductButton_clicked();


    void on_exitProductButton_clicked();

private:
    Ui::ProductDialog *ui;
    ProcessModel *processModel;
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *view;
    int mode;

    bool form2Entity(Entity *entity);
    void entity2Form(Entity *product);
    void clear();
};

#endif // PRODUCTDIALOG_H
