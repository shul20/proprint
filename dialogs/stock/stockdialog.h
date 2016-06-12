#ifndef STOCKDIALOG_H
#define STOCKDIALOG_H

#include <QDialog>
#include "entity/stock/stock.h"
#include "models/entitymodel.h"
#include <QSortFilterProxyModel>
#include "widgets/stock/productwidget.h"

namespace Ui {
class StockDialog;
}

class StockDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StockDialog(QWidget *parent);
    ~StockDialog();
    void set(QAbstractItemModel *model, int mode,
             const QModelIndex &index, int stockType);


private slots:
    void on_saveStockButton_clicked();

    void on_exitStockButton_clicked();

private:
    Ui::StockDialog *ui;
    ProductWidget *productW;
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *view;
    int mode;

    void entity2Form(Entity *stock);
    bool form2Entity(Entity *entity, QString &message);
    void setup(int type);
    void clear();
};

#endif // STOCKDIALOG_H
