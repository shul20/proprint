#include "taskwidget.h"
#include "entity/stock/price.h"
#include "utility/process.h"

TaskWidget::TaskWidget(QWidget *parent, int processId) :
    QWidget(parent), processId(processId)
{
    Process *process = Process::getInstance();
    QString color = QString("background: rgba(%1)").
                arg(process->getColor(processId).toString());

    setStyleSheet(QString("QToolButton {%1} "
                          "QGroupBox#checkGroup {%1}")
                  .arg(color));

    setObjectName(process->getName(processId).toString());

}

void TaskWidget::sizeChanged(){
    calculateSize();
}

void TaskWidget::priceTypeChanged(PriceType *type){
    priceTypeId = type->getId();

    calculatePrice();
    calculateTotal();
}

void TaskWidget::companyChanged(Company *company)
{
    companyId = company->getId();

    calculatePrice();
    calculateTotal();
}

void TaskWidget::stockChanged(Stock *stock)
{
    loadStockVar(stock);

    calculatePrice();
    calculateTotal();
}

void TaskWidget::productChanged(Product *product)
{
    productId = product->getId();
    calculatePrice();
    calculateTotal();
}

void TaskWidget::setWidgets(SizeWidget *sizeWidget, PriceTypeWidget *priceWidget,
                            CompanyWidget *companyWidget, TaskWidget *mainWidget){

    sizeW = sizeWidget;
    priceW = priceWidget;
    companyW = companyWidget;
    mainW = mainWidget;

    connect(sizeWidget, SIGNAL(sizeChanged()), this, SLOT(sizeChanged()));
    connect(priceWidget, SIGNAL(priceTypeChanged(PriceType*)),
            this, SLOT(priceTypeChanged(PriceType*)));
    connect(companyWidget, SIGNAL(companyChanged(Company*)),
            this, SLOT(companyChanged(Company*)));

    Company * company = companyWidget->getCompany();
    PriceType *priceType = priceWidget->getPriceType();
    if (company == NULL || priceType == NULL) {
        return;
    }
    companyId = company->getId();
    priceTypeId = priceType->getId();
}

Money &TaskWidget::getTotal()
{
    return total;
}

void TaskWidget::loadStockVar(Stock *stock)
{
    stockId = stock->getId();
    productId = stock->fields["product_id"].toInt();
    stockName = stock->fields["stock_name"].toString();
}

void TaskWidget::disconnectWidgets()
{
    disconnect(sizeW, SIGNAL(sizeChanged()), this, SLOT(sizeChanged()));
    disconnect(priceW, SIGNAL(priceTypeChanged(PriceType*)),
            this, SLOT(priceTypeChanged(PriceType*)));
    disconnect(companyW, SIGNAL(companyChanged(Company*)),
               this, SLOT(companyChanged(Company*)));
}

void TaskWidget::setTotal(MoneyEdit *field)
{
    if (total.isCustom) {
        if (sender() == (QObject *)field) {
            field->setCustomColor();
            emit totalChanged();
            return;
        }
        if (total == totalByPrice) {
            total.isCustom = false;
        }
    } else {
        total = totalByPrice;
    }

    field->setMoney(total);
    emit totalChanged();
}

void TaskWidget::totalInput(const QString &value)
{
    total = Money(value);
    total.isCustom = true;
    calculateTotal();
}

void TaskWidget::calculatePrice()
{
    price = Price::getPrice(companyId, productId, priceTypeId,
                                  processId, stockName);

    priceField->setMoney(price);
}
