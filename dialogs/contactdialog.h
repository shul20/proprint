#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include <QDebug>
#include "entity/contact.h"
#include "models/entitymodel.h"

namespace Ui {
class ContactDialog;
}

class ContactDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::ContactDialog *ui;
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *view;
    int mode;

    bool form2Entity(Entity *entity);
    void entity2Form(Entity *contact);
    void clear();
    
public:
    explicit ContactDialog(QWidget *parent);
    ~ContactDialog();
    void set(QAbstractItemModel *model, int mode, const QModelIndex &index);


private slots:
    void on_saveContactButton_clicked();
    void on_exitContactButton_clicked();
};

#endif // CONTACTDIALOG_H



