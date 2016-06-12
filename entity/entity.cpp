#include "entity.h"
#include <QDebug>
#include <QStringList>
#include <QSqlError>

const char Entity::retQueryTemplate[] = "select * from %1 where %2_id = :id";
const char Entity::delQueryTemplate[] = "delete from %1 where %1_id = :id";
const char Entity::insertQueryTemplate[] = "insert into %1 (%2) values (%3) returning %1_id";
const char Entity::updateQueryTemplate[] = "update %1 set (%2) = (%3) where %1_id = :id returning %1_id";
QString Entity::lastError = QString();

Entity::Entity() {}

Entity::Entity(Entity *other):
    tableName(other->tableName), tableView(other->tableView), fields(other->fields)

{}

Entity::~Entity()
{}

int Entity::getId()
{
    return fields[QString("%1_id").arg(*tableName)].toInt();
}

void Entity::clearId()
{
    fields.remove(QString("%1_id").arg(*tableName));
}

bool Entity::retrive(int id, bool isLock)
{
    QString retQuery = QString(retQueryTemplate).arg(*tableView, *tableName);
    if (isLock){
        retQuery.append(" for update nowait");
    }

    QSqlQuery query;
    query.prepare(retQuery);
    query.bindValue(":id", id);

    if (!query.exec()){
        return false;
    }

    query.next();
    setFields(query.record());

    return true;
}

bool Entity::retrive(const QString &field, QVariant value)
{

    QSqlQuery query;
    QString queryStr = QString("select * from %1 where %2 = :value")
            .arg(*tableName).arg(field);

    query.prepare(queryStr);
    query.bindValue(":value", value);

    if (!query.exec()){
        return false;
    }

    query.next();
    setFields(query.record());

    return true;
}

bool Entity::update()
{
    QString idStr = QString("%1_id").arg(*tableName);
    int id = fields[idStr].toInt();
    QStringList keys, values;
    QSqlQuery query;

    if (id > 0) {
        separateFields(keys, values);
        query.prepare(QString(updateQueryTemplate)
                      .arg(*tableName)
                      .arg(keys.join(", "))
                      .arg(values.join(", ")));

        query.bindValue(":id", id);

    } else {
        fields.remove(idStr);
        separateFields(keys, values);
        query.prepare(QString(insertQueryTemplate)
                      .arg(*tableName)
                      .arg(keys.join(", "))
                      .arg(values.join(", ")));
    }

    if (!query.exec()){
        errorMessage(query);
        return false;
    }
    query.next();

    id = query.record().value(0).toInt();
    fields[idStr] = id;
    return id != 0;
}

bool Entity::del()
{
    int id = fields[QString("%1_id").arg(*tableName)].toInt();

    if (id > 0) {
        QSqlQuery query;
        query.prepare(QString(delQueryTemplate).arg(*tableName));
        query.bindValue(":id", id);

        if (!query.exec()){
            errorMessage(query);
            return false;
        }
    }

    fields.clear();
    return true;
}

bool Entity::increment(const QString &field, QVariant value)
{
    QSqlQuery query;
    QString queryStr = QString("update %1 set %2 = %2 + :value where %1_id = :id returning %2")
            .arg(*tableName).arg(field);

    query.prepare(queryStr);
    query.bindValue(":value", value);
    query.bindValue(":id", getId());

    if (!query.exec()){
        return false;
    }

    query.next();
    fields[field] = query.record().value(0);
    return true;
}

bool Entity::increment(const QString &table, const QString &field,
                       QVariant value, int id)
{
    QSqlQuery query;
    QString queryStr = QString("update %1 set %1_%2 = %1_%2 + :value "
                               "where %1_id = :id")
            .arg(table).arg(field);

    query.prepare(queryStr);
    query.bindValue(":value", value);
    query.bindValue(":id", id);

    return query.exec();
}

void Entity::setFields(const QSqlRecord &record)
{
    fields.clear();
    for(int i = 0; i < record.count(); i++) {
        fields[record.fieldName(i)] = record.value(i);
    }
}

void Entity::separateFields(QStringList &keys, QStringList &values)
{
    for (QMap<QString, QVariant>::iterator i = fields.begin(); i != fields.end(); ++i){
            QVariant value = i.value();

            keys.append(i.key());
            if (!value.isNull()) {
                values.append(QString("'%1'").arg(value.toString()));
            } else {
                values.append(QString("NULL"));
            }
    }
}

void Entity::errorMessage(const QSqlQuery &query)
{
    QString database = query.lastError().databaseText();
    int index = database.indexOf("CONTEXT:");
    lastError = database.left(index);

#ifndef QT_DEBUG
    qDebug() << database;
#endif //QT_DEBUG
}

int Entity::checkEntity(int row, QAbstractItemModel *model){
    QModelIndex index = model->index(row, 0);
    Entity *entity = (Entity *) model->data(index, PPLib::entityRole).toInt();
    int id = entity->getId();
    if (id == 0) {
        model->setData(index, false, PPLib::delRole);
    } else {
        entity->retrive(id, false);
    }
    return id;
}
