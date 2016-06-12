#include "mounttaskf.h"
#include "ui_mounttaskf.h"
#include "models/productmodel.h"
#include "entity/order/task.h"
#include "models/modelmanager.h"
#include "entity/order/taskview.h"
#include "entity/order/order.h"
#include "entity/stock/woff.h"

MountTaskF::MountTaskF(QWidget *parent, int processId) :
    FlowWidget(parent),
    ui(new Ui::MountTaskF)
{
    ui->setupUi(this);

    startButton = ui->startButton;
    pauseButton = ui->pauseButton;
    readyButton = ui->readyButton;
    groupBox = ui->groupBox;

    ui->product->setup(true, true, true, false, false);
    ui->product->setRole(ProductModel::processRole, processId);

    disableFocus();

    connect(ui->product, SIGNAL(stockChanged()), this,
            SLOT(stockChanged()));

    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseClicked()));
    connect(readyButton, SIGNAL(clicked()), this, SLOT(readyClicked()));
}

MountTaskF::~MountTaskF()
{
    delete ui;
}

void MountTaskF::entity2Form(Entity *task)
{
    this->task = task;
    ui->product->blockSignals(true);
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
    ui->product->blockSignals(false);
}

void MountTaskF::disableFocus()
{
    ui->product->disableFocus();
    FlowWidget::disableFocus();
}

void MountTaskF::readyClicked()
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

void MountTaskF::stockChanged()
{
    Stock *stock = ui->product->getParam();
    if (stock == NULL) {
        return;
    }
    task->fields["stock_id"] = stock->getId();
    task->fields["product_id"] = stock->fields["product_id"];
}
