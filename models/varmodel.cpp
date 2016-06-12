#include "varmodel.h"
#include <QDebug>
#include <QDate>
#include "utility/money.h"
#include "entity/client.h"

const QStringList VarModel::header =
        QStringList() << QString::fromUtf8("Переменная")
                      << QString::fromUtf8("Параметр")
                      << QString::fromUtf8("Параметр")
                      << QString::fromUtf8("Изменена")
                      << QString::fromUtf8("Изменил");

VarModel::VarModel(QObject *parent) :
    EntityModel(parent), isRefresh(true)
{
    entityHeader = &header;

    QSqlQuery &query = refreshQuery;
    query.prepare(Var::allQuery);
}

QVariant VarModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole && (column == 2)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole && ((column == 1) || (column == 3) || (column == 4))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    int row = index.row();
    Var *var = static_cast<Var *>(getEntity(row));
    int id = var->getId() - 1;

    if (role == Qt::DisplayRole && column == 0) {
        return Var::vars[id];
    }

    if (column == 1 && ((role == Qt::EditRole) || (role == Qt::DisplayRole))) {
        if (id == Var::RATE) {
            Money value = var->fields["var_int"];
            return value.toString();
        }
        return var->fields["var_int"];
    }

    if ((column == 2) && ((role == Qt::EditRole) || (role == Qt::DisplayRole))) {
        return var->fields["var_string"];
    }

    if (role == Qt::DisplayRole && column == 3) {
        return var->fields["var_date_update"].toDate().toString(PPLib::dateTemplate);
    }

    if (role == Qt::DisplayRole && column == 4) {
        return var->fields["client_name"];
    }

    return EntityModel::data(index, role);
}

Var *VarModel::createEntity() const
{
        return new Var();
}

Qt::ItemFlags VarModel::flags(const QModelIndex &index) const
{
    int column = index.column();
    if ((column == 1) || (column == 2)) {
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }

    return QAbstractItemModel::flags(index);
}

bool VarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int column = index.column();

    if (role == Qt::EditRole) {
        int row = index.row();
        Var *var = static_cast<Var *>(getEntity(row));

        if (column == 1) {
            var->fields["var_int"] = value;
        }

        if (column == 2) {
            var->fields["var_string"] = value;
        }

        var->fields["client_name"] = Client::currentClient->fields["client_name"];
        var->fields["var_date_update"] = QDate::currentDate();

        if (!var->update()) {
            return false;
        }
        reset();
        return true;
    }

    return EntityModel::setData(index, value, role);
}

void VarModel::resetModel()
{
    reset();
}

void VarModel::setIsRefresh(bool check)
{
    isRefresh = check;
}

void VarModel::refresh()
{
    if (isRefresh) {
        EntityModel::refresh();
    }
}

QVariant VarModel::getVar(int varId)
{
    Var *var = static_cast<Var *>(getEntity(varId));
    return var->fields["var_int"];
}

void VarModel::setVar(int varId, int value)
{
    setData(this->index(varId, 1), value, Qt::EditRole);
}
