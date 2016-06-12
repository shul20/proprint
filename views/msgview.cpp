#include "msgview.h"
#include "ui_msgview.h"
#include "widgets/entitytable.h"
#include "dialogs/msgdialog.h"
#include "entity/client.h"

MsgView::MsgView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgView)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Сообщения"));

    EntityTable::setup(ui->table);
    //ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");

    model = new MessageModel(this);
    ui->table->setModel(model);

    QDate current = QDate::currentDate();
    ui->dateTill->blockSignals(true);
    ui->dateTill->setDate(current);
    ui->dateFrom->setDate(current.addMonths(-1));
    ui->dateTill->blockSignals(false);

    appendMenu();
}

MsgView::~MsgView()
{
    delete ui;
}

void MsgView::appendMenu()
{
    QAction *actionAdd, *actionRefresh, *actionDel;
    menu = new QMenu();

    actionAdd = new QAction(QString::fromUtf8("Создать"), this);
    connect(actionAdd, SIGNAL(triggered()), this, SLOT(addMessage()));

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()), model, SLOT(refresh()));

    actionDel = new QAction(QString::fromUtf8("Удалить"), this);
    connect(actionDel, SIGNAL(triggered()), this, SLOT(delMessage()));

    menu->addAction(actionAdd);
    menu->addAction(actionRefresh);
    if (Client::currentRole == Client::MANAGER ||
        Client::currentRole == Client::ADMIN) {

        menu->addAction(actionDel);
    }

    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editMessage(QModelIndex)));
}

void MsgView::on_table_customContextMenuRequested(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}

void MsgView::on_dateFrom_dateChanged(const QDate &date)
{
    refresh(date, ui->dateTill->date());
}

void MsgView::on_dateTill_dateChanged(const QDate &date)
{
    refresh(ui->dateFrom->date(), date);
}

void MsgView::addMessage()
{
    QModelIndex index;

    if (ui->table->selectionModel()->hasSelection()){
        index = ui->table->currentIndex();
    }

    MsgDialog *dialog = new MsgDialog(this);
    dialog->setModel(model, PPLib::addRole, index);
    dialog->exec();
    delete dialog;
}

void MsgView::delMessage()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if ((!select->hasSelection()) ||
        (PPLib::delRowMessage() == QMessageBox::Cancel)){
        return;
    }

    QModelIndexList indexes = ui->table->selectionModel()->selectedIndexes();
    while(indexes.size()) {
        if (!model->setData(indexes.first(), true, PPLib::delRole)) {
            return;

        } else {
            indexes = ui->table->selectionModel()->selectedIndexes();
        }
    }
}

void MsgView::editMessage(QModelIndex index)
{
    MsgDialog *dialog = new MsgDialog(this);
    dialog->setModel(model, Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}


void MsgView::refresh(QDate from, QDate till)
{
    QSqlQuery &query = model->getRefreshQuery();
    query.clear();

    if (Client::currentRole == Client::ADMIN) {
        query.prepare(Message::allQuery);
    } else {
        query.prepare(Message::clientQuery);
        query.bindValue(":client", Client::currentClient->getId());
    }

   query.bindValue(":from", from);
   query.bindValue(":till", till);
   query.bindValue(":status", Message::PERFORM);
    model->refresh();
}

Entity *MsgView::currentEntity()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return NULL;
    }
    return (Entity *)model->data(ui->table->currentIndex(),
                                 PPLib::entityRole).toInt();
}
