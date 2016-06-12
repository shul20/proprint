#include "equipmentmodel.h"
#include "utility/process.h"

const int EquipmentModel::processRole = PPLib::userRole + 1;

const QStringList EquipmentModel::header =
        QStringList() << QString::fromUtf8("Название")
                      << QString::fromUtf8("Процесс")
                      << QString::fromUtf8("Ширина")
                      << QString::fromUtf8("Статус");

EquipmentModel::EquipmentModel(QObject *parent):EntityModel(parent)
{
    entityHeader = &header;
}

QVariant EquipmentModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole && (column == 2)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    int row = index.row();
    Equipment *equip = static_cast<Equipment *>(getEntity(row));
    int status = equip->fields["equipment_status"].toInt();

    if (role == Qt::DisplayRole && column == 0) {
        return equip->fields["equipment_name"];
    }

    if (role == Qt::DisplayRole && column == 1) {
        int processId = equip->fields["equipment_process"].toInt();
        Process *process = Process::getInstance();
        return process->getName(processId);
    }

    if (role == Qt::DisplayRole && column == 2) {
        return equip->fields["equipment_width"];
    }

    if (role == Qt::DisplayRole && column == 3) {
        return Equipment::statuses[status];
    }
    if (role == processRole) {
        return equip->fields["equipment_process"];
    }

    if (role == Qt::BackgroundRole) {
       return QBrush(Equipment::colors[status]);
    }

    return EntityModel::data(index, role);
}

Equipment *EquipmentModel::createEntity() const
{
    return new Equipment();
}
