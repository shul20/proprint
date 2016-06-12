#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QWidget>
#include "models/sortfiltermodel.h"

namespace Ui {
class ProductView;
}

class ProductView : public QWidget
{
    Q_OBJECT
private:
    SortFilterModel *stockF;

public:
    explicit ProductView(QWidget *parent = 0);
    ~ProductView();

private slots:
    void on_stockType_currentIndexChanged(int index);
    void addProduct();
    void editProduct();
    void editProduct(QModelIndex index);

private:
    Ui::ProductView *ui;

};

#endif // PRODUCTVIEW_H
