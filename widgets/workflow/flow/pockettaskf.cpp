#include "pockettaskf.h"
#include "ui_pockettaskf.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"

PocketTaskF::PocketTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::PocketTaskF)
{
    ui->setupUi(this);

    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    ui->product->setup(true, false, false, false, false);
    ui->product->setRole(ProductModel::processRole, Process::POCKET);

    disableFocus();

    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(readyButton, SIGNAL(clicked()), this, SLOT(readyClicked()));
}

PocketTaskF::~PocketTaskF()
{
    delete ui;
}

void PocketTaskF::entity2Form(Entity *task)
{
    this->task = task;

    int productId = task->fields["product_id"].toInt();
    ui->product->setProduct(productId);

    ui->top->setValue(task->fields["task_ival_3"].toInt());
    ui->bottom->setValue(task->fields["task_ival_4"].toInt());

    status = task->fields["task_status"].toInt();
    isReady = (status == Task::READY);
    setStatus(status);
}

void PocketTaskF::disableFocus()
{
    ui->product->disableFocus();
    ui->product->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->top->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->top->setFocusPolicy(Qt::NoFocus);

    ui->bottom->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->bottom->setFocusPolicy(Qt::NoFocus);

    FlowWidget::disableFocus();
}
