#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QModelIndex>
#include <QAbstractItemModel>
#include "entity/client.h"
#include "models/entitymodel.h"

namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::ClientDialog *ui;
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *view;
    int mode;

    bool form2Entity(Entity *client);
    void entity2Form(Entity *client);
    void clear();

public:
    explicit ClientDialog(QWidget *parent);
    ~ClientDialog();
    void set(QAbstractItemModel *model, int mode, const QModelIndex &index);


private slots:
    void on_saveClientButton_clicked();

    void on_exitClientButton_clicked();
};

#endif // ClientDialog_H



