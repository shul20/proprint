#include "printflow.h"
#include "ui_printflow.h"
#include <QScrollArea>
#include "models/order/workmodel.h"
#include "widgets/entitytable.h"
#include "utility/process.h"
#include "widgets/workflow/flow/printtaskf.h"
#include "widgets/workflow/flow/colortaskf.h"
#include "widgets/workflow/flow/luverstaskf.h"
#include "widgets/workflow/flow/laminattaskf.h"
#include "widgets/workflow/flow/mounttaskf.h"
#include "widgets/workflow/flow/pockettaskf.h"
#include "widgets/workflow/flow/finishtaskf.h"
#include "widgets/workflow/flow/infotaskf.h"

PrintFlow::PrintFlow(QWidget *parent) :
    WorkFlowWidget(parent),
    ui(new Ui::PrintFlow), isReadOnly(false), scrollContent(NULL)
{
    ui->setupUi(this);

    taskMap[Process::PRINT] = new PrintTaskF();
    taskMap[Process::COLORPROOF] = new ColorTaskF();
    taskMap[Process::LUVERS] = new LuversTaskF();
    taskMap[Process::LAMINAT] = new LaminatTaskF();
    taskMap[Process::MOUNT] = new MountTaskF();
    taskMap[Process::POCKET] = new PocketTaskF();
    taskMap[Process::FINISHING] = new FinishTaskF();
    taskMap[Process::CUT] = new InfoTaskF(NULL, Process::CUT);
    taskMap[Process::WELD] = new InfoTaskF(NULL, Process::WELD);

    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(workSelected(QModelIndex)));

    EntityTable::setup(ui->table);
}

PrintFlow::~PrintFlow()
{
    delete ui;
}

void PrintFlow::setOrder(Entity *order)
{
    this->order = order;

    if (workModel != NULL) {
        delete workModel;
    }
    workModel = new WorkModel(this);
    workModel->setMode(WorkModel::PROCESS);

    int orderId = order->getId();
    QSqlQuery &query = workModel->getRefreshQuery();
    query.prepare(Work::allQuery);
    query.bindValue(":id", orderId);
    workModel->refresh();
    ui->table->setModel(workModel);

    QSqlQuery &queryTask = taskModel->getRefreshQuery();
    queryTask.prepare(Task::allByOrderQuery);
    queryTask.bindValue(":id", orderId);
    taskModel->refresh();
}

void PrintFlow::setReadOnly(bool set)
{
    isReadOnly = set;
    if (scrollContent != NULL) {
        scrollContent->setAttribute(Qt::WA_TransparentForMouseEvents, set);
    }
}

void PrintFlow::workSelected(QModelIndex index)
{
    delete ui->scrollFrame->layout()->takeAt(0);
    tasks.clear();

    QScrollArea *scrollArea = new QScrollArea();
    ui->scrollFrame->layout()->addWidget(scrollArea);
    scrollContent = new QWidget(scrollArea);

    QVBoxLayout *scrollLayout = new QVBoxLayout();
    scrollLayout->setMargin(2);
    scrollLayout->setSpacing(1);

    scrollContent->setAttribute(Qt::WA_TransparentForMouseEvents, isReadOnly);

    selectedWork = (Work *) workModel->data(index, PPLib::entityRole).toInt();

    taskF->setMatchValue(selectedWork->getId());
    for (int row = 0; row < taskF->rowCount(); row++) {
        QModelIndex index = taskF->index(row, 0);
        Task *task = (Task *) taskF->data(index, PPLib::entityRole).toInt();
        FlowWidget *taskWidget = taskMap[task->fields["task_process"].toInt()];

        taskWidget->set(order, selectedWork, taskF, workModel);

        scrollLayout->addWidget(taskWidget);
        tasks.append(taskWidget);

        taskWidget->entity2Form(task);
    }

    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding);
    scrollLayout->addItem(spacer);

    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
}

bool PrintFlow::update(Entity *order, QString &message)
{
    QList<Task *> taskList;

    for (int workRow = 0; workRow < workModel->rowCount(); workRow++) {
        QModelIndex workIndex = workModel->index(workRow, 0);
        Work *work = (Work *) workIndex.data(PPLib::entityRole).toInt();
        if (!work->update()){
            message = Work::lastError;
            return false;
        }

        taskF->setMatchValue(work->getId());

        for (int taskRow = 0; taskRow < taskF->rowCount(); taskRow++) {
            QModelIndex taskIndex = taskF->index(taskRow, 0);
            Task *task = (Task *) taskIndex.data(PPLib::entityRole).toInt();

            if (!task->update()) {
                message = Task::lastError;
                task->fields["task_status"] = Task::PAUSE;
                return false;
            }

            Woff *woff = (Woff *) task->getWoff();
            if (woff != NULL) {
                woff->fields["task_id"] = task->getId();

                if (!woff->update()) {
                    Stock *stock = (Stock *)woff->getStockEntity();
                    message = QString::fromUtf8("%1\n%2 %3 %4")
                            .arg(Woff::lastError)
                            .arg(stock->getProductName())
                            .arg(stock->fields["stock_name"].toString())
                            .arg(stock->fields["stock_param"].toInt());

                    return false;
                }

                taskList.append(task);
            }
        }
    }

    changeOrderStatus(order);

    foreach (Task *task, taskList) {
        Woff *woff = (Woff *) task->getWoff();
        Stock *stock = (Stock *)woff->getStockEntity();
        stock->retrive(stock->getId(), false);
        delete woff;
        task->setWoff(NULL);
    }

    return true;
}
