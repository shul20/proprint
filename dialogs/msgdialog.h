#ifndef MSGDIALOG_H
#define MSGDIALOG_H

#include <QDialog>
#include "models/messagemodel.h"
#include "entity/entity.h"

namespace Ui {
class MsgDialog;
}

class MsgDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MsgDialog(QWidget *parent = 0);
    ~MsgDialog();
    void setModel(QAbstractItemModel *model, int mode, const QModelIndex &index);
    
private slots:
    void on_exitButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MsgDialog *ui;
    QAbstractItemModel *model;
    Entity *view;
    QModelIndex index;
    int mode;

    void entity2Form(Entity *msg);
    bool form2Entity(Entity *msg);
};

#endif // MSGDIALOG_H
