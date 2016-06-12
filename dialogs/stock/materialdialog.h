#ifndef MATERIALDIALOG_H
#define MATERIALDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>

#include "entity/stock/material.h"
#include "entity/stock/stock.h"
#include "models/companymodel.h"
#include "models/stock/stockmodel.h"
#include "models/stock/materialmodel.h"
#include "widgets/stock/productwidget.h"

class MaterialModel;

namespace Ui {
class MaterialDialog;
}

class MaterialDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MaterialDialog(QWidget *parent);
    ~MaterialDialog();

    void set(QAbstractItemModel *model,  int mode, QModelIndex index);

private slots:
    void on_saveMaterialButton_clicked();

    void amountChanged(int);

    void stockChanged(Stock *);

    void on_price_textChanged(const QString &arg1);

    void on_length_valueChanged(int arg1);

    void on_exitMaterialButton_clicked();

private:
    Ui::MaterialDialog *ui;
    QAbstractItemModel *model;
    StockModel *stockModel;
    ProductWidget *productWidget;
    QSpinBox *amountBox;
    QLineEdit *sizeBox;
    int size, stockType, mode;
    QModelIndex index;

    void entity2Form(Stock *stock, Material *material);
    bool form2Entity(Entity *entity, QString &message);
    void setPermission(int mode);
    void setup(Stock *stock);
    void retriveContact();
    void retriveStock();

    void calculateTotal();
    void calculateSize();
};

#endif // MATERIALDIALOG_H
