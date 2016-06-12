#ifndef STOCKINFOW_H
#define STOCKINFOW_H

#include <QWidget>
#include "entity/stock/stock.h"

namespace Ui {
class StockInfoW;
}

class StockInfoW : public QWidget
{
    Q_OBJECT
    
public:
    explicit StockInfoW(QWidget *parent = 0);
    ~StockInfoW();

public slots:
    void stockChanged(Stock *stock);
    
private slots:
    void on_infoButton_clicked();

private:
    Ui::StockInfoW *ui;
    Stock *stock;
};

#endif // STOCKINFOW_H
