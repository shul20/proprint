#include "paymodel.h"
#include "entity/finance/account.h"
#include "entity/client.h"

const int PayModel::materialRole = PPLib::userRole + 1;
const int PayModel::debtMaterialRole = PPLib::userRole + 2;
const int PayModel::otherRole = PPLib::userRole + 3;
const int PayModel::debtOtherRole = PPLib::userRole + 4;
const int PayModel::orderRole = PPLib::userRole + 5;
const int PayModel::debtOrderRole = PPLib::userRole + 6;
const int PayModel::stockRole = PPLib::userRole + 7;

const QStringList PayModel::header =
        QStringList() << QString::fromUtf8("Название")
                      << QString::fromUtf8("Тип")
                      << QString::fromUtf8("Счет источник")
                      << QString::fromUtf8("Счет приемник")
                      << QString::fromUtf8("Залог");

PayModel::PayModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
    QSqlQuery &query = refreshQuery;
    query.prepare(Pay::allQuery);
}

QVariant PayModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole && (column != 0)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    int row = index.row();
    if (row < 0) {
        return QVariant();
    }
    Pay *pay = static_cast<Pay *>(getEntity(row));
    int type = pay->fields["pay_type"].toInt();
    int debt = pay->fields["pay_debt"].toInt();

    if ((column == 0) && ((role == Qt::DisplayRole) || role == (Qt::EditRole))) {
        return pay->fields["pay_name"];
    }

    if ((column == 1) && (role == Qt::DisplayRole)) {
        return Pay::types[type];
    }

    if ((column == 2) && (role == Qt::DisplayRole)) {
        QVariant account = pay->fields["src_account"];
        if (account.isNull()){
            return QString("---");
        }
        int sign = pay->fields["src_sign"].toInt();
        return QString::fromUtf8("%1 (%2)")
                .arg(Account::accounts[account.toInt() - 1])
                .arg(getCreditName(sign));
    }

    if ((column == 3) && (role == Qt::DisplayRole)) {
        QVariant account = pay->fields["dst_account"];
        if (account.isNull()){
            return QString("---");
        }
        int sign = pay->fields["dst_sign"].toInt();
        return QString::fromUtf8("%1 (%2)")
                .arg(Account::accounts[account.toInt() - 1])
                .arg(getCreditName(sign));
    }


    if ((column == 4) && (role == Qt::DisplayRole)) {
        return Pay::deposits[debt];
    }

    int clientRole = Client::currentRole;
    int permission = pay->fields["pay_permission"].toInt();

    if (role == materialRole){
        return type == Pay::MATERIAL &&
               (debt == Pay::NODEBT || debt == Pay::CREATEDEBT) &&
               clientRole <= permission;
    }

    if (role == orderRole){
        return type == Pay::ORDER &&
               (debt == Pay::NODEBT || debt == Pay::CREATEDEBT) &&
               clientRole <= permission;
    }

    if (role == debtOrderRole){
        return type == Pay::ORDER &&
               (debt == Pay::CHANGEDEBT || debt == Pay::WOFFDEBT) &&
               clientRole <= permission;
    }

    if (role == debtMaterialRole){
        return type == Pay::MATERIAL &&
               (debt == Pay::CHANGEDEBT || debt == Pay::WOFFDEBT) &&
               clientRole <= permission;
    }

    if (role == debtOtherRole){
        return type == Pay::OTHER &&
               (debt == Pay::CHANGEDEBT || debt == Pay::WOFFDEBT) &&
               clientRole <= permission;
    }

    if (role == otherRole){
        return type == Pay::OTHER &&
               debt != Pay::CHANGEDEBT &&
               clientRole <= permission;
    }

    if (role == stockRole){
        return type == Pay::STOCK &&
               debt != Pay::CHANGEDEBT &&
               clientRole <= permission;
    }


    return EntityModel::data(index, role);
}

Pay *PayModel::createEntity() const
{
    return new Pay();
}


const QString &PayModel::getCreditName(int sign) const
{
    if (sign < 0){
        return Pay::credits[Pay::CREDIT];
    } else {
        return Pay::credits[Pay::DEBET];
    }
}
