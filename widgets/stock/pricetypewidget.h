#ifndef PRICETYPEWIDGET_H
#define PRICETYPEWIDGET_H

#include <QWidget>
#include "entity/stock/pricetype.h"
namespace Ui {
class PriceTypeWidget;
}

class PriceTypeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PriceTypeWidget(QWidget *parent = 0);
    ~PriceTypeWidget();
     void setup(bool isButton);

     void setType(int priceTypeId);
     PriceType *getPriceType();
    
private slots:
    void on_type_currentIndexChanged(int index);

    void on_addPriceType_clicked();

    void on_delPriceType_clicked();

private:
    Ui::PriceTypeWidget *ui;

signals:
    void priceTypeChanged(PriceType *);
};

#endif // PRICETYPEWIDGET_H
