#include "printwidget.h"
#include "ui_printwidget.h"
#include "utility/process.h"
#include "widgets/work/task/luvers/luverstask.h"
#include "widgets/work/task/laminattask.h"
#include "widgets/work/task/mounttask.h"
#include "widgets/work/task/pockettask.h"
#include "widgets/work/task/infotask.h"
#include "widgets/work/task/finishtask.h"
#include "widgets/entitytable.h"

PrintWidget::PrintWidget(QWidget *parent) :
    WorkWidget(parent),
    ui(new Ui::PrintWidget)
{
    ui->setupUi(this);
    EntityTable::setup(ui->table);
    ui->table->setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

    sizeW = ui->size;
    mainProcess = Process::PRINT;
    mainWidget = ui->print;

    taskMap[Process::PRINT] = ui->print;
    taskMap[Process::COLORPROOF] = new InfoTask(this, Process::COLORPROOF, false);
    taskMap[Process::LUVERS] = new LuversTask(this);
    taskMap[Process::LAMINAT] = new LaminatTask(this);
    taskMap[Process::MOUNT] = new MountTask(this);
    taskMap[Process::POCKET] = new PocketTask(this);
    taskMap[Process::CUT] = new InfoTask(this, Process::CUT);
    taskMap[Process::WELD] = new InfoTask(this, Process::WELD);
    taskMap[Process::FINISHING] = new FinishTask(this);

    QList<TaskWidget *> tasksList = taskMap.values();
    tasksList.removeAt(0);
    ui->tab->setData((QList<QWidget *>&) tasksList);

    setupMenu();
}

PrintWidget::~PrintWidget()
{
    delete ui;
}

void PrintWidget::clear()
{
    if (workModel != NULL) {
        delete workModel;
    }
    workModel = new WorkModel(this);
    workModel->setMode(0);
    ui->table->setModel(workModel);
    taskF->setMatchValue(0);
    emit worksChaged();
}



ProductWidget *PrintWidget::getProductW()
{
    return ui->print->getProductW();
}

Entity *PrintWidget::getEquip()
{
    return ui->print->getEquip();
}

void PrintWidget::setOrder(int orderId, bool isCopy)
{
    if (workModel != NULL) {
        delete workModel;
    }
    workModel = new WorkModel(this);
    workModel->setMode(0);

    QSqlQuery &query = workModel->getRefreshQuery();
    query.prepare(Work::allQuery);
    query.bindValue(":id", orderId);
    workModel->refresh();
    ui->table->setModel(workModel);

    QSqlQuery &queryTask = taskModel->getRefreshQuery();
    queryTask.prepare(Task::allByOrderQuery);
    queryTask.bindValue(":id", orderId);
    taskModel->refresh();

    if (isCopy) {
        resetId();
    }
}


void PrintWidget::on_clearButton_clicked()
{
    ui->size->clear();
    WorkWidget::clearTasks();
}

void PrintWidget::addWork()
{
    int works = workModel->rowCount();

    Work *work = new Work();
    work->fields["work_id"] = idCounter;
    if (!ui->size->form2Entity(work) ||
        !addTasks2Work(work) ||
        !workModel->setData(QModelIndex(), (int) work, PPLib::addRole)){
        delete work;
        return;
    }

    total = total + workTotal;
    totalPrint = totalPrint + workTotalPrint;

    idCounter -= 1;
    totalSize += Size(work->fields["work_size"]);

    emit worksChaged();

    if (works == 0) {
        emit blockInput(true);
    }
}

void PrintWidget::delWork()
{
    if (!ui->table->selectionModel()->hasSelection()){
           return;
    }

    Entity *work = (Entity *) workModel->data(ui->table->currentIndex(),
                                              PPLib::entityRole).toInt();
    if (!delTasksFromWork(work) ||
        !workModel->setData(ui->table->currentIndex(), false, PPLib::delRole)) {
        return;
    }

    totalSize -= Size(work->fields["work_size"]);
    total = total - Money(work->fields["work_total"]);
    totalPrint = totalPrint - Money(work->fields["work_totalprint"]);

    emit worksChaged();

    if (workModel->rowCount() == 0) {
        emit blockInput(false);
    }
}

void PrintWidget::refreshWorks()
{
    workModel->setData(QModelIndex(), QVariant(), PPLib::refreshRole);
}

void PrintWidget::modifyWork(QModelIndex index)
{
    Entity *work = (Entity *) workModel->data(index, PPLib::entityRole).toInt();
    ui->size->entity2Form(work);

    clearTasks();
    tasks2Forms(work->getId());
}

void PrintWidget::editWork()
{
    Size oldSize;
    Money oldTotalWork, oldTotalPrintWork;

    if (!ui->table->selectionModel()->hasSelection()){
           return;
    }

    Entity *work = (Entity *) workModel->data(ui->table->currentIndex(),
                                              PPLib::entityRole).toInt();

    oldSize = Size(work->fields["work_size"]);
    oldTotalWork = Money(work->fields["work_total"]);
    oldTotalPrintWork = Money(work->fields["work_totalprint"]);

    if (!ui->size->form2Entity(work) ||
        !delTasksFromWork(work) ||
        !addTasks2Work(work)){
        return;
    }

    totalSize += Size(work->fields["work_size"]) - oldSize;
    total = total + Money(work->fields["work_total"]) - oldTotalWork;
    totalPrint = totalPrint + Money(work->fields["work_totalprint"]) - oldTotalPrintWork;

    emit worksChaged();
}

void PrintWidget::totalChanged()
{
    calcWorkTotal();
    ui->total->setMoney(workTotal);
    ui->totalPrint->setMoney(workTotalPrint);
    ui->totalPostPrint->setMoney(workTotal - workTotalPrint);
}

bool PrintWidget::update(int orderId)
{
    if (workModel->rowCount() == 0) {
        return false;
    }

    if (!taskModel->delRemoved() || !workModel->delRemoved()) {
        return false;
    }

    for (int row = 0; row < workModel->rowCount(); row++){
        QModelIndex indx = workModel->index(row, 0);
        Work *work = (Work *) workModel->data(indx, PPLib::entityRole).toInt();

        int oldWorkId = work->getId();

        work->fields["ordr_id"] = orderId;
        if (!work->update()){
            return false;
        }

        taskF->setMatchValue(oldWorkId);
        int newWorkId = work->getId();
        if (!TaskModel::update(taskF, newWorkId)) {
            return false;
        }
    }
    return true;
}

void PrintWidget::setupMenu()
{
    QAction *actionAdd, *actionEdit, *actionRefresh, *actionDel;
    menu = new QMenu();
    actionAdd = new QAction(QString::fromUtf8("Добавить"), this);
    connect(actionAdd, SIGNAL(triggered()),this, SLOT(addWork()));

    actionEdit = new QAction(QString::fromUtf8("Изменить"), this);
    connect(actionEdit, SIGNAL(triggered()),this, SLOT(editWork()));

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refreshWorks()));

    actionDel = new QAction(QString::fromUtf8("Удалить"), this);
    connect(actionDel, SIGNAL(triggered()),this, SLOT(delWork()));

    menu->addAction(actionAdd);
    menu->addAction(actionEdit);
    menu->addAction(actionRefresh);
    menu->addAction(actionDel);

    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(modifyWork(QModelIndex)));
}

void PrintWidget::on_table_customContextMenuRequested(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}
