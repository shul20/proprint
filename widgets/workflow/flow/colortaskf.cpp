#include "colortaskf.h"
#include "ui_colortaskf.h"
#include "models/equip/equipmentmodel.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "entity/equip/pmode.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"


ColorTaskF::ColorTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::ColorTaskF)
{
    ui->setupUi(this);
    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    ui->product->setup(true, true, true, true, false);
    ui->product->setRole(ProductModel::processRole, Process::PRINT);

    ui->equip->setup(true, true, true, true);
    ui->equip->setEquipRole(EquipmentModel::processRole, Process::PRINT);

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

ColorTaskF::~ColorTaskF()
{
    delete ui;
}

void ColorTaskF::entity2Form(Entity *task)
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

    ui->info->setText(task->fields["task_info"].toString());
    ui->type->setCurrentIndex(task->fields["task_ival_3"].toInt());

    status = task->fields["task_status"].toInt();
    isReady = (status == Task::READY);
    setStatus(status);

    ui->equip->blockSignals(false);
    ui->product->blockSignals(false);
}


void ColorTaskF::disableFocus()
{
     ui->product->disableFocus();
     ui->equip->disableFocus();
     ui->type->setFocusPolicy(Qt::NoFocus);
     FlowWidget::disableFocus();
}

void ColorTaskF::equipChanged()
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

void ColorTaskF::stockChanged()
{
    Stock *stock = ui->product->getParam();
    if (stock == NULL) {
        return;
    }
    task->fields["stock_id"] = stock->getId();
    task->fields["product_id"] = stock->fields["product_id"];
}

void ColorTaskF::typeChanged(int type)
{
    task->fields["task_ival_3"] = type;
}

void ColorTaskF::readyClicked()
{
    Client *client = Client::currentClient;
    if (client->currentRole != Client::MANAGER &&
        client->currentRole != Client::ADMIN) {

        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
           QString::fromUtf8("Цветопробу подтверждает менеджер"),
                              QMessageBox::Ok);

        return;
    }

    FlowWidget::readyClicked();
}
