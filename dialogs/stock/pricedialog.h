#ifndef PRICEDIALOG_H
#define PRICEDIALOG_H

#include <QDialog>
#include "entity/stock/price.h"
#include "widgets/stock/productwidget.h"

namespace Ui {
class PriceDialog;
}

class PriceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PriceDialog(QWidget *parent = 0);
    ~PriceDialog();
    void set(QAbstractItemModel *model, int mode, const QModelIndex &index);
    
private slots:
    void on_savePriceButton_clicked();

    void on_process_currentIndexChanged(int index);

    void on_exitPriceButton_clicked();

private:
    Ui::PriceDialog *ui;
    ProductWidget *productW;
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *view;
    int mode;

    void entity2Form(Entity *price);
    bool form2Entity(Entity *entity);
    void clear();
};

#endif // PRICEDIALOG_H
