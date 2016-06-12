#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include "entity/entity.h"
#include "utility/money.h"
#include "widgets/stock/productwidget.h"
#include "widgets/work/task/sizewidget.h"
#include "widgets/companywidget.h"
#include "widgets/stock/pricetypewidget.h"
#include "utility/moneyedit.h"

class TaskWidget : public QWidget
{
    Q_OBJECT
protected:
    SizeWidget *sizeW;
    PriceTypeWidget *priceW;
    CompanyWidget  *companyW;
    TaskWidget *mainW;
    Stock *stock;
    int priceTypeId, companyId, productId, stockId, processId;
    QString stockName;
    Money total, totalByPrice, price;
    MoneyEdit *priceField;

    void loadStockVar(Stock *stock);
    void disconnectWidgets();
    void setTotal(MoneyEdit *field);
    void calculatePrice();

public:
    explicit TaskWidget(QWidget *parent, int processId);
    virtual void setWidgets(SizeWidget *sizeWidget, PriceTypeWidget *priceWidget,
                            CompanyWidget *companyWidget, TaskWidget *mainWidget);


    virtual void clear() = 0;
    virtual void entity2Form(Entity *entity) = 0;
    virtual bool form2Entity(Entity *entity) = 0;
    //virtual Money total() = 0;
    virtual void calculateSize() = 0;
    virtual void calculateTotal() = 0;
    Money &getTotal();

private slots:
    void sizeChanged();
    void priceTypeChanged(PriceType *type);
    void companyChanged(Company *company);
    void stockChanged(Stock *stock);
    void productChanged(Product*product);
    void totalInput(const QString &value);

signals:
    void setCheck(bool);
    void totalChanged();
    void nameChanged();
};

#endif // TASKWIDGET_H
