#include "laminattaskf.h"
#include "ui_laminattaskf.h"
#include "models/equip/equipmentmodel.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"
#include "entity/stock/woff.h"

LaminatTaskF::LaminatTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::LaminatTaskF)
{
    ui->setupUi(this);
    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    ui->product->setup(true, true, true, true, false);
    ui->product->setRole(ProductModel::processRole, processId);

    ui->equip->setup(true, false, false, false);
    ui->equip->setEquipRole(EquipmentModel::processRole, processId);

    disableFocus();

    connect(ui->equip, SIGNAL(equipChanged()), this,
            SLOT(equipChanged()));

    connect(ui->product, SIGNAL(stockChanged()), this,
            SLOT(stockChanged()));

    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(readyButton, SIGNAL(clicked()), this, SLOT(readyClicked()));
}

LaminatTaskF::~LaminatTaskF()
{
    delete ui;
}

void LaminatTaskF::entity2Form(Entity *task)
{
    this->task = task;
    ui->equip->blockSignals(true);
    ui->product->blockSignals(true);

    int equipId = task->fields["equipment_id"].toInt();
    ui->equip->setEquip(equipId);

    int stockId = task->fields["stock_id"].toInt();
    int productId = task->fields["product_id"].toInt();
    ui->product->setParam(productId, stockId);
    Stock *stock = ui->product->getParam();
    if (stock != NULL) {
        ui->woff->setup(stock);
    }

    status = task->fields["task_status"].toInt();
    setStatus(status);
    if (status == Task::READY) {
        ui->woff->woffShow(task);
        isReady = true;
    }
    ui->equip->blockSignals(false);
    ui->product->blockSignals(false);
}

void LaminatTaskF::disableFocus()
{
    ui->product->disableFocus();
    ui->equip->disableFocus();
    FlowWidget::disableFocus();
}

void LaminatTaskF::equipChanged()
{
    Equipment *equip = ui->equip->getEquip();
    if (equip == NULL) {
        return;
    }
    task->fields["equipment_id"] = equip->getId();
}

void LaminatTaskF::stockChanged()
{
    Stock *stock = ui->product->getParam();
    if (stock == NULL) {
        return;
    }
    task->fields["stock_id"] = stock->getId();
    task->fields["product_id"] = stock->fields["product_id"];
}

void LaminatTaskF::readyClicked()
{
    QString message;
    Stock *stock = ui->product->getParam();
    Task *task = (Task *) this->task;
    Woff *woff = ui->woff->createWoff(work, stock, message);
    if (woff == NULL) {
        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), message);
        return;
    }
    woff->fields["ordr_id"] = order->getId();
    woff->fields["client_id"] = Client::currentClient->getId();
    woff->setStockEntity(stock);
    task->setWoff(woff);

    FlowWidget::readyClicked();
}
