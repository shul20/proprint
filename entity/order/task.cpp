#include "task.h"

const QString Task::table("task");

const char Task::allByOrderQuery[] =
        "select * from task where work_id in "
        "(select work_id from work where ordr_id = :id ) "
        "order by task_weight;";

const QStringList Task::statuses = QStringList()
        << QString::fromUtf8("готов")
        << QString::fromUtf8("подготовка")
        << QString::fromUtf8("работа")
        << QString::fromUtf8("пауза");

const QVector<QColor> Task::statusesColor = QVector<QColor>()
        << QColor(179,189,231)
        << QColor(192,192,192)
        << QColor(191,220,195)
        << QColor(252,202,202);

const QStringList Task::typeList = QStringList()
        << QString::fromUtf8("Двунаправленная")
        << QString::fromUtf8("Однонаправленная")
        << QString::fromUtf8("Двусторонняя");

void Task::setFields(const QSqlRecord &record)
{
    Entity::setFields(record);
    isReady = (fields["task_status"] == Task::READY);

}

Task::Task():Entity(), woff(NULL)
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

Task::Task(Task *other):Entity(other), woff(other->woff)
{}

Entity *Task::getWoff()
{
    return woff;
}

void Task::setWoff(Entity *entity)
{
    woff = entity;
}

bool Task::getIsReady()
{
    return isReady;
}

void Task::setIsReady(bool value)
{
    isReady = value;
}

Size Task::getSizeOfTasks(int stockId, int taskStatus)
{
    QSqlQuery query;
    query.prepare("select sum(work_size) from work where work_id in "
                  "(select work_id from task where "
                  "stock_id = :id and task_status != :status);");

    query.bindValue(":id", stockId);
    query.bindValue(":status", taskStatus);

    if (!query.exec() or !query.next()) {
        return Size();
    }

    return Size(query.record().value(0));
}
