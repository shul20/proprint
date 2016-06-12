#include "contactmodel.h"
#include "utility/money.h"

const int ContactModel::companyIdRole = PPLib::userRole + 1;

const QStringList ContactModel::header =
        QStringList() << QString::fromUtf8("Имя")
                      << QString::fromUtf8("Email")
                      << QString::fromUtf8("Телефон")
                      << QString::fromUtf8("Долг, грн");

ContactModel::ContactModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole && (column == 3)) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    int row = index.row();
    if (row < 0) {
        return QVariant();
    }
    Contact *contact = static_cast<Contact*>(getEntity(row));

    if (role == Qt::DisplayRole && column == 0) {
        return QString("%1 %2")
                .arg(contact->fields["contact_name"].toString())
                .arg(contact->fields["contact_surname"].toString());
    }

    if (role == Qt::DisplayRole && column == 1) {
        return contact->fields["contact_email"];
    }

    if (role == Qt::DisplayRole && column == 2) {
        return contact->fields["contact_tel"];
    }

    if (role == Qt::DisplayRole && column == 3) {
        int cents = contact->fields["contact_debt"].toInt();
        return Money(-cents).toString();
    }

    if (role == companyIdRole) {
        return contact->fields["company_id"];
    }

    return EntityModel::data(index, role);
}

QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return entityHeader->at(section);

    } else {
        return QVariant();
    }
}

Contact *ContactModel::createEntity() const
{
    return new Contact();
}

bool ContactModel::update(QAbstractItemModel *model, int companyId)
{
    if (model->rowCount() == 0) {
        return false;
    }
    for (int row = 0; row < model->rowCount(); row++){
        QModelIndex index = model->index(row, 0);
        Contact *contact = (Contact *) model->data(index, PPLib::entityRole).toInt();
        contact->fields["company_id"] = companyId;
        if (!contact->update()){
            return false;
        }
    }
    return true;
}

bool ContactModel::update(int companyId)
{
    foreach (Entity *entity, entities) {
        entity->fields["company_id"] = companyId;

        if (!entity->update()) {
            return false;
        }
    }
    return true;
}
