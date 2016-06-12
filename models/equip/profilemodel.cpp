#include "profilemodel.h"

int ProfileModel::passRole = PPLib::userRole + 1;

const QStringList ProfileModel::header =
        QStringList() << QString::fromUtf8("Имя");

ProfileModel::ProfileModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant ProfileModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    int row = index.row();
    Profile *profile = static_cast<Profile *>(getEntity(row));

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && column == 0) {
        return profile->fields["profile_name"];
    }

    if (role == passRole) {
        return profile->fields["pass_id"];
    }

    return EntityModel::data(index, role);
}

bool ProfileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int column = index.column();

    if (role == Qt::EditRole) {
        int row = index.row();
        Profile *profile = static_cast<Profile *>(getEntity(row));

        if (column == 0) {
            profile->fields["profile_name"] = value;

        }
        return true;
    }

    return EntityModel::setData(index, value, role);
}

QVariant ProfileModel::headerData(int section, Qt::Orientation orientation, int role) const
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


Qt::ItemFlags ProfileModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

Profile *ProfileModel::createEntity() const
{
    return new Profile();
}
