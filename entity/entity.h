#ifndef ENTITY_H
#define ENTITY_H

#include <QList>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>
#include "models/entitymodel.h"

class Entity
{
private:
    static const char retQueryTemplate[];
    static const char delQueryTemplate[];
    static const char insertQueryTemplate[];
    static const char updateQueryTemplate[];

    void separateFields(QStringList &keys, QStringList &values);
    void errorMessage(const QSqlQuery &query);

protected:
    const QString *tableName, *tableView;

public:
    Entity();
    Entity(Entity *other);
    virtual ~Entity();

    int getId();
    void clearId();

    virtual void setFields(const QSqlRecord &record);
    virtual bool retrive(int id, bool isLock);
    virtual bool retrive(const QString &field, QVariant value);
    virtual bool update();
    virtual bool del();

    bool increment(const QString &field, QVariant value);
    static bool increment(const QString &table, const QString &field, QVariant value, int id);
    static int checkEntity(int row, QAbstractItemModel *model);

    QMap<QString, QVariant> fields;
    static QString lastError;
};


Q_DECLARE_METATYPE(Entity)

#endif // ENTITY_H
