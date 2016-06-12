#include "pmodemodel.h"

int PmodeModel::equipIdRole = PPLib::userRole + 1;

const QStringList PmodeModel::header =
        QStringList() << QString::fromUtf8("Режим")
                      << QString::fromUtf8("X dpi")
                      << QString::fromUtf8("Y dpi");

PmodeModel::PmodeModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant PmodeModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    int row = index.row();
    Pmode *pmode = static_cast<Pmode *>(getEntity(row));

    if (role == Qt::DisplayRole && column == 0) {
        return QString("%1 x %2 dpi")
                .arg(pmode->fields["pmode_x"].toInt())
                .arg(pmode->fields["pmode_y"].toInt());
    }

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && column == 1) {
        return pmode->fields["pmode_x"];
    }

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && column == 2) {
        return pmode->fields["pmode_y"];
    }

    if (role == equipIdRole) {
        return pmode->fields["equipment_id"];
    }

    return EntityModel::data(index, role);
}

bool PmodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int column = index.column();
    if (role == Qt::EditRole) {
        int row = index.row();
        Pmode *pmode = static_cast<Pmode *>(getEntity(row));
        if (column == 1) {
            pmode->fields["pmode_x"] = value.toInt();

        } else if (column == 2) {
            pmode->fields["pmode_y"] = value.toInt();
        }
        return true;
     }

     return EntityModel::setData(index, value, role);
}


Qt::ItemFlags PmodeModel::flags(const QModelIndex &index) const
{
    int column = index.column();
    if (column == 0) {
        return QAbstractItemModel::flags(index);
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

Pmode *PmodeModel::createEntity() const
{
    return new Pmode();
}
