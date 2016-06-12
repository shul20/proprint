#include "equipdialog.h"
#include "ui_equipdialog.h"
#include "models/processmodel.h"
#include "mainwindow.h"

EquipDialog::EquipDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::EquipDialog), equipW(NULL)
{
    ui->setupUi(this);

    for (int i = 0; i < Equipment::statuses.length(); i++){
        ui->status->addItem(Equipment::statuses[i], i);
    }

    ui->process->blockSignals(true);
    ProcessModel *processModel = new ProcessModel(this, Process::ISEQUIP);
    ui->process->setModel(processModel);
    ui->process->blockSignals(false);
}

EquipDialog::~EquipDialog()
{
    delete ui;
}

void EquipDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
{
    this->model = model;
    this->index = index;
    this->mode = mode;

    if (mode == PPLib::addRole) {
        if (index.isValid()){
            view = (Entity *) model->data(index, PPLib::entityRole).toInt();
            entity2Form(view);
        } else {
            clear();
        }
        ui->saveEquipButton->setText(QString::fromUtf8("Добавить"));
        setWindowTitle(QString::fromUtf8("Добавить оборудование"));

    } else if (mode == Qt::EditRole) {
        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);
        ui->saveEquipButton->setText(QString::fromUtf8("Изменить"));
        setWindowTitle(QString::fromUtf8("Редактирование оборудования"));
        ui->process->setDisabled(true);
    }
}

void EquipDialog::on_saveEquipButton_clicked()
{
    Equipment *saveEquip = NULL;
    MainWindow::db->transaction();

    if (mode == PPLib::addRole) {
        saveEquip = new Equipment();

    } else if (mode == Qt::EditRole) {
        saveEquip = new Equipment((Equipment *)view);
    }

    if (!form2Entity(saveEquip) ||
        !saveEquip->update() ||
        !equipW->update(saveEquip->getId())) {

        MainWindow::db->rollback();
        delete saveEquip;
        return;
    }

    MainWindow::db->commit();
    model->setData(index, (int) saveEquip, mode);
    QDialog::accept();
}

bool EquipDialog::form2Entity(Entity *entity)
{
    QString name = ui->name->text();
    int width = ui->width->value();
    if ((name.length() == 0) || (width == 0)){
        PPLib::fillFormMessage();
        return false;
    }

    entity->fields["equipment_name"] = name;
    entity->fields["equipment_width"] = width;
    entity->fields["equipment_process"] = ui->process->itemData(ui->process->currentIndex(), PPLib::idRole);
    entity->fields["equipment_status"] = ui->status->currentIndex();

    return true;
}

void EquipDialog::entity2Form(Entity *equip)
{
    ui->name->setText(equip->fields["equipment_name"].toString());
    ui->width->setValue(equip->fields["equipment_width"].toInt());

    int processId = equip->fields["equipment_process"].toInt();
    int index = ui->process->findData(processId, PPLib::idRole);

    if (index == ui->process->currentIndex()){
        on_process_currentIndexChanged(index);
    } else {
        ui->process->setCurrentIndex(index);
    }

    int status = equip->fields["equipment_status"].toInt();
    ui->status->setCurrentIndex(status);
}

void EquipDialog::clear()
{
    ui->name->clear();
    ui->width->setValue(0);
    ui->status->setCurrentIndex(0);

    if (ui->process->currentIndex() == 0){
        on_process_currentIndexChanged(0);
    } else {
        ui->process->setCurrentIndex(0);
    }
}


void EquipDialog::on_process_currentIndexChanged(int index)
{
    if (equipW != NULL) {
        ui->equip->layout()->removeWidget(equipW);
        delete equipW;
        equipW = NULL;
    }

    int processId = ui->process->itemData(index, PPLib::idRole).toInt();

    if (processId == Process::PRINT){
        equipW = new PrinterWidget(this);
        ui->equip->layout()->addWidget(equipW);

        if (this->index.isValid()){
            int printerId = model->data(this->index, PPLib::idRole).toInt();
            equipW->setPrinter(printerId);
        }
    }

    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void EquipDialog::on_exitEquipButton_clicked()
{
    if (equipW != NULL) {
        equipW->exitNoUpdate();
    }
    QDialog::reject();
}
