#ifndef STOCKWIDGET_H
#define STOCKWIDGET_H

#include <QWidget>
#include "models/productmodel.h"
#include "models/stock/stockmodel.h"
#include "models/sortfiltermodel.h"
#include "widgets/stock/productwidget.h"

namespace Ui {
class StockWidget;
}

class StockWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StockWidget(QWidget *parent = 0);
    ~StockWidget();

    void set(int type);
    
private:
    Ui::StockWidget *ui;
    int stockType;
    SortFilterModel *stockF;
    ProductWidget *productW;

    void appendMenu();

private slots:
    void addStock();
    void editStock();
    void editStock(QModelIndex index);
    void addMaterial();
    void rollCut();
    void woffMaterial();
    void on_checkBox_toggled(bool checked);
    void productChanged(Product *product);
};

#endif // STOCKWIDGET_H
