#ifndef MESSAGE_H
#define MESSAGE_H

#include "entity/entity.h"

class Message : public Entity
{
private:
    static const QString table, tableView;
    QVariant msgFrom, msgTo;

public:
    Message();
    Message(Message *other);
    void setFields(const QSqlRecord &record);
    bool update();

    QVariant &getMsgFrom();
    void setMesgFrom(const QVariant &value);

    QVariant &getMsgTo();
    void setMesgTo(const QVariant &value);

    static const char allQuery[], clientQuery[], checkNewMsg[];

    static const QStringList statuses;
    static const enum {NEWMSG = 0, READ = 1, REJECT = 2, PERFORM = 3} statusEnum;

    static int checkNewMessage(int clientId);
};

#endif // MESSAGE_H
