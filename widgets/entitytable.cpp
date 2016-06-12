#include "entitytable.h"
#include <QHeaderView>
#include <QDebug>
#include "proprintlib.h"

EntityTable::EntityTable(QWidget *parent) :
    QTableView(parent), dialog(NULL)
{
    setSortingEnabled(true);

    setup(this);

}

Entity *EntityTable::currentEntity()
{
    QItemSelectionModel *select = selectionModel();

    if (!select->hasSelection()){
        return NULL;
    }
    return (Entity *)model()->data(currentIndex(), PPLib::entityRole).toInt();
}

QMenu *EntityTable::getMenu()
{
    return menu;
}

void EntityTable::setDialog(EntityDialog *dialog)
{
    this->dialog = dialog;
}

void EntityTable::setSortingEnabled(bool enable)
{
    if (!enable){
        setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");
    }

    QTableView::setSortingEnabled(enable);
}

void EntityTable::setup(QTableView *table)
{
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setContextMenuPolicy(Qt::CustomContextMenu);

    QHeaderView *verticalHeader = table->verticalHeader();
    verticalHeader->setResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(24);

    table->setAlternatingRowColors(true);
    table->setStyleSheet("QTableView { alternate-background-color: #EEE }");

    QHeaderView *horizontalHeader = table->horizontalHeader();
    horizontalHeader->setResizeMode(QHeaderView::ResizeToContents);
}

void EntityTable::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    setupMenu();

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(customContextMenu(const QPoint&)));
}


void EntityTable::customContextMenu(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}

void EntityTable::refresh()
{
    model()->setData(QModelIndex(), QVariant(), PPLib::refreshRole);
}


void EntityTable::del()
{
    QItemSelectionModel *select = this->selectionModel();

    if ((!select->hasSelection()) ||
        (PPLib::delRowMessage() == QMessageBox::Cancel)){
        return;
    }

    QModelIndexList indexes = selectionModel()->selectedIndexes();
    while(indexes.size()) {
        if (!model()->setData(indexes.first(), true, PPLib::delRole)) {
            return;

        } else {
            indexes = selectionModel()->selectedIndexes();
        }
    }
}


void EntityTable::setupMenu()
{
    menu = new QMenu();
    actionAdd = new QAction(QString::fromUtf8("Добавить"), this);

    actionEdit = new QAction(QString::fromUtf8("Изменить"), this);

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refresh()));

    actionDel = new QAction(QString::fromUtf8("Удалить"), this);
    connect(actionDel, SIGNAL(triggered()), this, SLOT(del()));

    menu->addAction(actionAdd);
    menu->addAction(actionEdit);
    menu->addAction(actionRefresh);
    menu->addAction(actionDel);
}
