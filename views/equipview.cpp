#include "equipview.h"
#include "ui_equipview.h"
#include "models/equip/equipmentmodel.h"
#include "dialogs/equip/equipdialog.h"
#include "models/modelmanager.h"

EquipView::EquipView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EquipView)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Оборудование"));
    ModelManager *models = ModelManager::getInstance();

    EquipmentModel *model = models->getEquipModel();
    ui->table->setModel(model);
    ui->table->setSortingEnabled(false);

    connect(ui->table->actionAdd, SIGNAL(triggered()), this, SLOT(addEquip()));
    connect(ui->table->actionEdit, SIGNAL(triggered()), this, SLOT(editEquip()));
    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editEquip(QModelIndex)));

}

EquipView::~EquipView()
{
    delete ui;
}

void EquipView::addEquip()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    EquipDialog *dialog = new EquipDialog(this);
    if (!select->hasSelection()){
        dialog->set(ui->table->model(), PPLib::addRole, QModelIndex());
    } else {
        dialog->set(ui->table->model(), PPLib::addRole, select->currentIndex());
    }

    dialog->exec();
    delete dialog;
}

void EquipView::editEquip()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editEquip(select->currentIndex());
}

void EquipView::editEquip(QModelIndex index)
{
    EquipDialog *dialog = new EquipDialog(this);
    dialog->set(ui->table->model(), Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}
