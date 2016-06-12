#ifndef PAYDIALOG_H
#define PAYDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>
#include "entity/finance/pay.h"
#include <QGroupBox>
#include <QComboBox>

namespace Ui {
class PayDialog;
}

class PayDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PayDialog(QWidget *parent = 0);
    ~PayDialog();
    void set(QAbstractItemModel *model, int mode, const QModelIndex &index);
    
private slots:

    void on_exitButton_clicked();

    void on_saveButton_clicked();

    void on_colorButton_clicked();

private:
    Ui::PayDialog *ui;
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *view;
    int mode;

    bool form2Entity(Entity *pay);
    void entity2Form(Entity *pay);
    void clear();
    void paintButton(QColor &color);
    int getSign(QComboBox *box);
    void setSign(QComboBox *box, int sign);
};

#endif // PAYDIALOG_H
