#ifndef STOCKVIEW_H
#define STOCKVIEW_H

#include <QWidget>
#include "models/stock/stockmodel.h"
#include "models/stock/materialmodel.h"

namespace Ui {
class StockView;
}

class StockView : public QWidget
{
    Q_OBJECT
    
public:
    explicit StockView(QWidget *parent = 0);
    ~StockView();

    QAbstractItemModel * getMaterialModel();
    int currentMaterialStock();

    static StockView *instance;
    
private:
    Ui::StockView *ui;

private slots:

};

#endif // STOCKVIEW_H
