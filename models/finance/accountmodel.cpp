#include "accountmodel.h"

const QStringList AccountModel::header =
        QStringList() << QString::fromUtf8("Название")
                      << QString::fromUtf8("Наличный счет")
                      << QString::fromUtf8("Безналичный счет");

AccountModel::AccountModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
    QSqlQuery &query = refreshQuery;
    query.prepare(Account::allQuery);
}

Account *AccountModel::createEntity() const
{
    return new Account();
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    int row = index.row();
    if (row < 0) {
        return QVariant();
    }
    Account *account = static_cast<Account *>(getEntity(row));
    int id = account->getId() - 1;
    if ((column == 0) && (role == Qt::DisplayRole)) {
        return Account::accounts[id];
    }

    return EntityModel::data(index, role);
}
