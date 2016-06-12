#include "luverstaskf.h"
#include "ui_luverstaskf.h"
#include "models/equip/equipmentmodel.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "entity/equip/pmode.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"


LuversTaskF::LuversTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::LuversTaskF)
{
    ui->setupUi(this);
    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    ui->product->setup(true, true, false, false, false);
    ui->product->setRole(ProductModel::processRole, Process::LUVERS);

    ui->control->setAttribute(Qt::WA_TransparentForMouseEvents);
    disableFocus();

    connect(ui->product, SIGNAL(stockChanged()), this,
            SLOT(stockChanged()));

    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(readyButton, SIGNAL(clicked()), this, SLOT(readyClicked()));
}

LuversTaskF::~LuversTaskF()
{
    delete ui;
}

void LuversTaskF::entity2Form(Entity *task)
{
    this->task = task;
    ui->product->blockSignals(true);

    int checks = task->fields["task_ival_3"].toInt();
    qlonglong values = task->fields["task_lval_1"].toLongLong();
    ui->control->setData(checks, values);
    ui->luversAmount->setValue(task->fields["task_lval_2"].toInt());

    int productId = task->fields["product_id"].toInt();
    int stockId = task->fields["stock_id"].toInt();
    ui->product->setMaterial(productId, stockId);

    status = task->fields["task_status"].toInt();
    isReady = (status == Task::READY);
    setStatus(status);

    ui->product->blockSignals(false);
}


void LuversTaskF::disableFocus()
{
     ui->product->disableFocus();
     FlowWidget::disableFocus();
}

void LuversTaskF::stockChanged()
{
    Stock *stock = ui->product->getMaterial();
    if (stock == NULL) {
        return;
    }
    task->fields["stock_id"] = stock->getId();
    task->fields["product_id"] = stock->fields["product_id"];
}
