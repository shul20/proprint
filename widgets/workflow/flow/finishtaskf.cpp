#include "finishtaskf.h"
#include "ui_finishtaskf.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"

FinishTaskF::FinishTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::FinishTaskF)
{
    ui->setupUi(this);

    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    ui->product->setup(true, false, false, false, false);
    ui->product->setRole(ProductModel::processRole, Process::FINISHING);

    disableFocus();

    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(readyButton, SIGNAL(clicked()), this, SLOT(readyClicked()));
}

FinishTaskF::~FinishTaskF()
{
    delete ui;
}

void FinishTaskF::entity2Form(Entity *task)
{
    this->task = task;
    int productId = task->fields["product_id"].toInt();
    ui->product->setProduct(productId);

    int checks = task->fields["task_ival_3"].toInt();

    ui->top->setChecked((bool)(checks & 1));
    ui->down->setChecked((bool)((checks & 2) >> 1));
    ui->left->setChecked((bool)((checks & 4) >> 2));
    ui->right->setChecked((bool)((checks & 8) >> 3));

    status = task->fields["task_status"].toInt();
    isReady = (status == Task::READY);
    setStatus(status);

}

void FinishTaskF::disableFocus()
{
    ui->product->disableFocus();
    ui->product->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->top->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->top->setFocusPolicy(Qt::NoFocus);

    ui->down->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->down->setFocusPolicy(Qt::NoFocus);

    ui->left->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->left->setFocusPolicy(Qt::NoFocus);

    ui->right->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->right->setFocusPolicy(Qt::NoFocus);

    FlowWidget::disableFocus();
}
