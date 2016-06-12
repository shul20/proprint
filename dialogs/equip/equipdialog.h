#ifndef EQUIPDIALOG_H
#define EQUIPDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

#include "entity/equip/equipment.h"
#include "dialogs/equip/printerwidget.h"

namespace Ui {
class EquipDialog;
}

class EquipDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EquipDialog(QWidget *parent);
    ~EquipDialog();
    void set(QAbstractItemModel *model, int mode, const QModelIndex &index);


private slots:
    void on_saveEquipButton_clicked();

    void on_process_currentIndexChanged(int index);

    void on_exitEquipButton_clicked();

private:
    Ui::EquipDialog *ui;
    PrinterWidget *equipW;
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *view;
    int mode;


    bool form2Entity(Entity *entity);
    void entity2Form(Entity *equip);
    void clear();
    bool update(Equipment* equip, const QModelIndex &index, int role);

};

#endif // EQUIPDIALOG_H
