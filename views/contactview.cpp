#include "contactview.h"
#include "ui_contactview.h"
#include "entity/contact.h"
#include "dialogs/contactdialog.h"
#include "models/modelmanager.h"
#include "widgets/entitytable.h"

ContactView::ContactView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactView), newContacts(NULL)
{
    ui->setupUi(this);
    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

    ModelManager *models = ModelManager::getInstance();
    contactF = new SortFilterModel(this);
    contactF->setFilterRole(ContactModel::companyIdRole);
    contactF->setSourceModel(models->getContactModel());
    ui->table->setModel(contactF);

    appendMenu();
}

ContactView::~ContactView()
{
    delete ui;
}

void ContactView::setCompany(int companyId)
{
    contactF->setMatchValue(companyId);
}

bool ContactView::update(int companyId)
{
    EntityModel *model = (EntityModel *) contactF->sourceModel();

    return model->delRemoved() && ContactModel::update(contactF, companyId);
}

void ContactView::exitNoUpdate()
{
    EntityModel *model = (EntityModel *) contactF->sourceModel();
    model->unDelete();

    for (int row = contactF->rowCount() - 1; row >= 0; row--){
        Entity::checkEntity(row, contactF);
    }
}

void ContactView::addContact()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    ContactDialog *dialog = new ContactDialog(this);
    if (!select->hasSelection()){
        dialog->set(ui->table->model(), PPLib::addRole, QModelIndex());
    } else {
        dialog->set(ui->table->model(), PPLib::addRole, select->currentIndex());
    }

    dialog->exec();
    delete dialog;
}

void ContactView::editContact()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editContact(select->currentIndex());
}

void ContactView::editContact(QModelIndex index)
{
    ContactDialog *dialog = new ContactDialog(this);
    dialog->set(ui->table->model(), Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}

void ContactView::delContact()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }
    if (contactF->rowCount() <= 1) {
        return;
    }
    contactF->setData(ui->table->currentIndex(), false, PPLib::delRole);
}


void ContactView::appendMenu()
{
    QAction *actionAdd, *actionEdit, *actionRefresh, *actionDel;
    menu = new QMenu();

    actionAdd = new QAction(QString::fromUtf8("Добавить"), this);
    connect(actionAdd, SIGNAL(triggered()), this, SLOT(addContact()));

    actionEdit = new QAction(QString::fromUtf8("Изменить"), this);
    connect(actionEdit, SIGNAL(triggered()), this, SLOT(editContact()));

    actionRefresh = new QAction(QString::fromUtf8("Обновить"), this);
    connect(actionRefresh, SIGNAL(triggered()),
            (EntityModel *)contactF->sourceModel(),SLOT(refresh()));

    actionDel = new QAction(QString::fromUtf8("Удалить"), this);
    connect(actionDel, SIGNAL(triggered()), this, SLOT(delContact()));

    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editContact(QModelIndex)));

    menu->addAction(actionAdd);
    menu->addAction(actionEdit);
    menu->addAction(actionRefresh);
    menu->addAction(actionDel);
}

void ContactView::on_table_customContextMenuRequested(const QPoint &pos)
{
    menu->exec(QCursor().pos());
}
