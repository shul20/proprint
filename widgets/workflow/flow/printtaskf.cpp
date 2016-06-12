#include "printtaskf.h"
#include "ui_printtaskf.h"
#include "models/equip/equipmentmodel.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "entity/equip/pmode.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"
#include "entity/stock/woff.h"


PrintTaskF::PrintTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::PrintTaskF)
{
    ui->setupUi(this);
    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    ui->product->setup(true, true, true, true, false);
    ui->product->setRole(ProductModel::processRole, processId);

    ui->equip->setup(true, true, true, true);
    ui->equip->setEquipRole(EquipmentModel::processRole, processId);

    for (int i = 0; i < Task::typeList.length(); i++){
        ui->type->addItem(Task::typeList[i], i);
    }
    disableFocus();

    connect(ui->equip, SIGNAL(equipChanged()), this,
            SLOT(equipChanged()));

    connect(ui->product, SIGNAL(stockChanged()), this,
            SLOT(stockChanged()));

    connect(ui->type, SIGNAL(currentIndexChanged(int)), this,
            SLOT(typeChanged(int)));

    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(readyButton, SIGNAL(clicked()), this, SLOT(readyClicked()));
}

PrintTaskF::~PrintTaskF()
{
    delete ui;
}

void PrintTaskF::entity2Form(Entity *task)
{
    this->task = task;
    ui->equip->blockSignals(true);
    ui->product->blockSignals(true);

    int equipId = task->fields["equipment_id"].toInt();
    int modeId = task->fields["task_ival_4"].toInt();
    int passId = task->fields["task_ival_5"].toInt();
    int profileId = task->fields["task_ival_6"].toInt();
    ui->equip->setProfile(equipId, modeId, passId, profileId);

    int stockId = task->fields["stock_id"].toInt();
    int productId = task->fields["product_id"].toInt();
    ui->product->setParam(productId, stockId);
    Stock *stock = ui->product->getParam();
    if (stock != NULL) {
        ui->woff->setup(stock);
    }

    ui->path->setText(task->fields["task_info"].toString());
    ui->type->setCurrentIndex(task->fields["task_ival_3"].toInt());

    status = task->fields["task_status"].toInt();
    setStatus(status);
    if (status == Task::READY) {
        ui->woff->woffShow(task);
        isReady = true;
    }

    ui->equip->blockSignals(false);
    ui->product->blockSignals(false);
}

void PrintTaskF::disableFocus()
{
     ui->product->disableFocus();
     ui->equip->disableFocus();
     ui->type->setFocusPolicy(Qt::NoFocus);
     FlowWidget::disableFocus();
}

void PrintTaskF::equipChanged()
{
    Equipment *equip = ui->equip->getEquip();
    Pmode *mode = ui->equip->getMode();
    Profile *profile = ui->equip->getProfile();

    if (equip == NULL || mode == NULL || profile == NULL) {
        return;
    }
    task->fields["equipment_id"] = equip->getId();
    task->fields["task_ival_4"] = mode->getId();
    task->fields["task_ival_5"] = profile->fields["pass_id"];
    task->fields["task_ival_6"] = profile->getId();
}

void PrintTaskF::stockChanged()
{
    Stock *stock = ui->product->getParam();
    if (stock == NULL) {
        return;
    }
    task->fields["stock_id"] = stock->getId();
    task->fields["product_id"] = stock->fields["product_id"];
}

void PrintTaskF::typeChanged(int type)
{
    task->fields["task_ival_3"] = type;
}

void PrintTaskF::readyClicked()
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
