#include "infotaskf.h"
#include "ui_infotaskf.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"

InfoTaskF::InfoTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::InfoTaskF)
{
    ui->setupUi(this);

    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    disableFocus();

    Process *process = Process::getInstance();
    groupBox->setTitle(process->getName(processId).toString());

    ui->product->setup(true, false, false, false, false);
    ui->product->setRole(ProductModel::processRole, processId);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(readyButton, SIGNAL(clicked()), this, SLOT(readyClicked()));
}

InfoTaskF::~InfoTaskF()
{
    delete ui;
}

void InfoTaskF::entity2Form(Entity *task)
{
    this->task = task;

    //int productId = task->fields["product_id"].toInt();
    //ui->product->setProduct(productId);

    ui->info->setText(task->fields["task_info"].toString());

    status = task->fields["task_status"].toInt();
    isReady = (status == Task::READY);
    setStatus(status);
}

void InfoTaskF::disableFocus()
{
    ui->product->disableFocus();
    ui->product->setAttribute(Qt::WA_TransparentForMouseEvents);

    FlowWidget::disableFocus();
}
