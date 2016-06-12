#include "messagemodel.h"
#include <QDateTime>

const QStringList MessageModel::header =
        QStringList() << QString::fromUtf8("От:")
                      << QString::fromUtf8("Дата")
                      << QString::fromUtf8("Кому:")
                      << QString::fromUtf8("Тема")
                      << QString::fromUtf8("Статус");

MessageModel::MessageModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    int row = index.row();

    Message *msg = static_cast<Message *>(getEntity(row));

    if (role == Qt::DisplayRole && column == 0) {
        return msg->getMsgFrom();
    }

    if (role == Qt::DisplayRole && column == 1) {
        return msg->fields["message_date"].toDateTime()
                .toString(PPLib::dateTimeTemplate);
    }

    if (role == Qt::DisplayRole && column == 2) {
        return msg->getMsgTo();
    }

    if (role == Qt::DisplayRole && column == 3) {
        return msg->fields["message_subj"].toString().left(100);
    }

    if (role == Qt::DisplayRole && column == 4) {
        int status = msg->fields["message_status"].toInt();
        return Message::statuses[status];
    }

    return EntityModel::data(index, role);
}

Message *MessageModel::createEntity() const
{
    return new Message();
}
