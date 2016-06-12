#include "stockinfow.h"
#include "ui_stockinfow.h"
#include "utility/size.h"
#include "entity/order/task.h"
#include "widgets/stock/productwidget.h"

StockInfoW::StockInfoW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockInfoW), stock(NULL)
{
    ui->setupUi(this);
}

StockInfoW::~StockInfoW()
{
    delete ui;
}

void StockInfoW::stockChanged(Stock *stock)
{
    if (!isVisible()){
        return;
    }
    this->stock = stock;
    int type = stock->fields["stock_type"].toInt();
    if (type == Stock::ROLL) {
        Size stockSize(stock->fields["stock_amount"]);
        ui->stock->setText(QString::fromUtf8("%1 кв.м").arg(stockSize.toString()));

    } else if (type == Stock::LIST) {
        ui->stock->setText(QString::fromUtf8("%1 шт.")
                           .arg(stock->fields["stock_amount"].toInt()));
    }

}

void StockInfoW::on_infoButton_clicked()
{
    if (stock == NULL) {
        return;
    }
    Size stockSize(stock->fields["stock_amount"]);
    int stockId = stock->getId();
    Size reserved = Task::getSizeOfTasks(stockId, Task::READY);
    int type = stock->fields["stock_type"].toInt();

    if (type == Stock::ROLL) {
        ui->reserv->setText(reserved.toString());
        ui->available->setText((stockSize - reserved).toString());

    }


}
