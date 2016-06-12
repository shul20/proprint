#include "passmodel.h"

int PassModel::modeRole = PPLib::userRole + 1;

const QStringList PassModel::header =
        QStringList() << QString::fromUtf8("Пасы")
                      << QString::fromUtf8("Скорость bidi")
                      << QString::fromUtf8("Скорость uni");

PassModel::PassModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant PassModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    int row = index.row();
    Pass *pass = static_cast<Pass *>(getEntity(row));

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && column == 0) {
        return pass->fields["pass_name"];
    }

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && column == 1) {
        return pass->fields["pass_speed_bidi"].toReal();
    }

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && column == 2) {
        return pass->fields["pass_speed_uni"].toReal();
    }

    if (role == modeRole) {
        return pass->fields["pmode_id"];
    }

    return EntityModel::data(index, role);
}

bool PassModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int column = index.column();

    if (role == Qt::EditRole) {
        int row = index.row();
        Pass *pass = static_cast<Pass *>(getEntity(row));
        if (column == 0) {
            pass->fields["pass_name"] = value;

        } else if (column == 1) {
            pass->fields["pass_speed_bidi"] = value;

        } else if (column == 2) {
            pass->fields["pass_speed_uni"] = value;
        }
        return true;

    }
    return EntityModel::setData(index, value, role);
}

Qt::ItemFlags PassModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

Pass *PassModel::createEntity() const
{
    return new Pass();
}
QVariant PassModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return entityHeader->at(section);

    } else {
        return QString("*");
    }
}
