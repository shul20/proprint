#include "taskspwidget.h"
#include "ui_taskspwidget.h"
#include "widgets/entitytable.h"
#include "entity/order/order.h"
#include "entity/order/taskview.h"
#include "dialogs/order/orderprocessd.h"
#include <QTimer>

TasksPWidget::TasksPWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TasksPWidget)
{
    ui->setupUi(this);
    EntityTable::setup(ui->table);
    ui->table->setSortingEnabled(false);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
}

TasksPWidget::~TasksPWidget()
{
    delete ui;
}

void TasksPWidget::setup(QAbstractTableModel *model, const QString &label, InfoWidget *info)
{
    ui->table->setModel(model);
    ui->label->setText(label);

    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)), this,
            SLOT(editOrder(QModelIndex)));

    connect(ui->table, SIGNAL(clicked(QModelIndex)), info,
            SLOT(setOrder(QModelIndex)));
}

void TasksPWidget::editOrder(QModelIndex index)
{
    QAbstractItemModel *model = ui->table->model();

    TaskView *task = (TaskView *) model->data(index,
                                           PPLib::entityRole).toInt();

    if (task == NULL) {
        return;
    }

    OrderProcessD *dialog = new OrderProcessD(this);
    dialog->setModel(model, task);
    dialog->exec();

    delete dialog;
}
