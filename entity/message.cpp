#include "message.h"

const QString Message::table("message");
const QString Message::tableView("message_view");

const QStringList Message::statuses = QStringList()
        << QString::fromUtf8("Новое")
        << QString::fromUtf8("Прочитано")
        << QString::fromUtf8("Отклонено")
        << QString::fromUtf8("Выполнено");

const char Message::allQuery[] = "select * from message_view "
        "where ((date(message_date) between :start and :end) "
        "or message_status != :status) "
        "order by message_date desc";

const char Message::clientQuery[] = "select * from message_view "
        "where client_to = :client and "
        "((date(message_date) between :start and :end) or message_status != :status) "
        "order by message_date desc;";

const char Message::checkNewMsg[] = "select count(*) from message "
        "where client_to = :client and message_status = :status;";

Message::Message():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Message::Message(Message *other):Entity(other)
{
    msgFrom = other->msgFrom;
    msgTo = other->msgTo;
}

void Message::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);
    msgFrom = fields["message_from"];
    msgTo = fields["message_to"];
}

bool Message::update()
{
    fields.remove("message_from");
    fields.remove("message_to");

    return Entity::update();
}

QVariant &Message::getMsgFrom()
{
    return msgFrom;
}

void Message::setMesgFrom(const QVariant &value)
{
    msgFrom = value;
}

QVariant &Message::getMsgTo()
{
    return msgTo;
}

void Message::setMesgTo(const QVariant &value)
{
    msgTo = value;
}

int Message::checkNewMessage(int clientId)
{
    QSqlQuery query;
    query.prepare(checkNewMsg);
    query.bindValue(":client", clientId);
    query.bindValue(":status", NEWMSG);

    if (!query.exec() || !query.next()) {
        return 0;
    }
    return query.record().value(0).toInt();
}
