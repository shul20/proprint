#include "woffwidget.h"
#include "ui_woffwidget.h"
#include "entity/stock/stock.h"
#include "models/modelmanager.h"
#include "models/varmodel.h"
#include <QDateTime>

QString smallStock = QString::fromUtf8("Не достаточно материала на складе");
QString bigSpend = QString::fromUtf8("Расход материала меньше объема заказа");

WoffWidget::WoffWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WoffWidget), work(NULL)
{
    ui->setupUi(this);
}

WoffWidget::~WoffWidget()
{
    delete ui;
}

void WoffWidget::setup(Stock *stock)
{
    this->stock = stock;
    int stockType = stock->fields["stock_type"].toInt();
    if (stockType == Stock::ROLL) {
        ui->spendUnit->setText(QString::fromUtf8("м.п."));
        ui->wasteUnit->setText(QString::fromUtf8("м.п."));

    } else if (stockType == Stock::LIST) {
        ui->spendUnit->setText(QString::fromUtf8("л."));
        ui->wasteUnit->setText(QString::fromUtf8("л."));
    }
    ui->spend->setText("0.0");
    ui->waste->setText("0.0");
}

bool WoffWidget::woffPrepare(Entity *work, Entity *stock, QString &message){

    return true;
}

Woff *WoffWidget::createWoff(Entity *work, Entity *stock, QString &message)
{
    int stockType = stock->fields["stock_type"].toInt();
    this->work = work;
    this->stock = stock;
    Woff *woff = NULL;
    int woffLevel = ModelManager::getInstance()->getVarModel()->getVar(Var::OVERRUN).toInt();

    if (stockType == Stock::ROLL) {
        Size workSize = work->fields["work_size"];
        Size stockSize = stock->fields["stock_amount"];
        int rollWidth = stock->fields["stock_param"].toInt();
        Size materialSpend = Size(rollWidth, ui->spend->getSize().value * 10);

        if (stockSize < materialSpend) {
            message = smallStock;
            return woff;
        }

        if (materialSpend < workSize) {
            message = bigSpend;
            return woff;
        }

        woff = new Woff();
        Size materialWaste = Size(rollWidth, ui->waste->getSize().value * 10);

        Size woffNorma = (workSize * woffLevel).value / 100;
        Size overRun = materialSpend - workSize - woffNorma;

        if (overRun < 0) {
            overRun = 0;
        }
        woff->fields["woff_over"] = overRun.value;
        woff->fields["woff_amount"] = (materialSpend + materialWaste).value;
        woff->fields["woff_waste"] = materialWaste.value;

    } else if (stockType == Stock::LIST) {
        int stockSize = stock->fields["stock_amount"].toInt();
        int workAmount = work->fields["work_amount"].toInt();
        int spend = ui->spend->text().toInt();

        if (stockSize < spend) {
            message = smallStock;
            return woff;
        }
        if (spend < workAmount) {
            message = bigSpend;
            return woff;
        }

        woff = new Woff();
        int amount = work->fields["work_size"].toInt();
        int waste = ui->waste->text().toInt();
        int woffNorma = (amount * woffLevel) / 100;
        int overRun = spend - amount - woffNorma;

        if (overRun < 0) {
            overRun = 0;
        }

        woff->fields["woff_over"] = overRun;
        woff->fields["woff_amount"] = spend + waste;
        woff->fields["woff_waste"] = waste;
    }

    woff->fields["stock_id"] = stock->getId();
    woff->fields["woff_date"] = QDateTime::currentDateTime();

    return woff;
}

bool WoffWidget::update(Woff *woff)
{

}

void WoffWidget::woffShow(int ){

}

void WoffWidget::woffShow(Entity *entity)
{
    //Woff *woff = Woff::getWoff(task);

    Task *task = (Task *) entity;
    Woff *woff = (Woff *)task->getWoff();
    if (woff == NULL) {
        woff = Woff::getWoff(task->getId());
    }
    //Entity *stock = woff->getStockEntity();

    int stockType = stock->fields["stock_type"].toInt();
    if (stockType == Stock::ROLL) {
        int size = woff->fields["woff_amount"].toInt();
        int waste = woff->fields["woff_waste"].toInt();
        int rollWidth = stock->fields["stock_param"].toInt();

        qreal runMeters = (qreal) ( size - waste) * 10 / rollWidth;

        ui->spend->setText(QString::number(runMeters));
        ui->waste->setText(Size(waste).toString());

    } else if (stockType == Stock::LIST) {
        int amount = woff->fields["woff_amount"].toInt();
        int waste = woff->fields["woff_waste"].toInt();

        ui->spend->setText(QString::number(amount - waste));
        ui->waste->setText(QString::number(waste));
    }

    //delete woff;
}
