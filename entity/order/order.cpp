#include "order.h"
#include "entity/client.h"
#include "dialogs/order/orderdialog.h"
#include "dialogs/order/orderprocessd.h"

const QString Order::table("ordr");
const QString Order::tableView("ordr");

const char Order::viewByStatus[] = "select * from ordr_view "
                                  "where ordr_status in (%1) "
                                  "order by equipment_id, ordr_priority";

const char Order::viewArch[] =
        "select * from ordr_viewarch "
        "where ordr_status = :status and "
        "(ordr_debt > 0 or (date(ordr_release) between :from and :till)) "
        "order by ordr_release desc;";

const char Order::searchStoryArch[] =
        "select * from ordr_viewarch "
        "where ordr_status = :status and "
        "ordr_story like :search "
        "order by ordr_release desc;";

const QStringList Order::statuses = QStringList()
        << QString::fromUtf8("удален")
        << QString::fromUtf8("создание")
        << QString::fromUtf8("в работу")
        << QString::fromUtf8("пауза")
        << QString::fromUtf8("процесс")
        << QString::fromUtf8("постпроцесс")
        << QString::fromUtf8("готов")
        << QString::fromUtf8("архив");

const QStringList Order::deliveries = QStringList()
                    << QString::fromUtf8("Самовывоз")
                    << QString::fromUtf8("Офис")
                    << QString::fromUtf8("Такси")
                    << QString::fromUtf8("Новая Почта");

const QStringList Order::packs = QStringList()
                    << QString::fromUtf8("Смотать в рулон")
                    << QString::fromUtf8("Смотать на тубу")
                    << QString::fromUtf8("Инфо:");

Order::Order():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &tableView;
}

Order::Order(Order *other): Entity(other)
{}

bool Order::setPriority(int orderId, int priority)
{
    QSqlQuery query;

    query.prepare(QString("update ordr set ordr_priority = :priority where ordr_id = :id"));

    query.bindValue(":priority", priority);
    query.bindValue(":id", orderId);

    if (!query.exec()) {
        return false;
    }
    return true;
}

bool Order::setStatus(int orderId, int status)
{
    QSqlQuery query;

    query.prepare(QString("update ordr set ordr_status = :status where ordr_id = :id"));

    query.bindValue(":status", status);
    query.bindValue(":id", orderId);

    if (!query.exec()) {
        return false;
    }
    return true;
}

void Order::openOrder(QAbstractItemModel *model, int mode, Entity *orderView)
{
    if (orderView->fields["ordr_id"].toInt() == 0) {
        return;
    }
    int managerId = orderView->fields["client_id"].toInt();

    if (Client::isOwner(managerId)){
        OrderDialog *dialog = new OrderDialog();
        dialog->setModel(model, mode, orderView);
        dialog->exec();
        delete dialog;
        return;
    }
    PPLib::accessDeniedMessage();
}

void Order::openProcess(QAbstractItemModel *model, Entity *orderView)
{
    if (orderView->fields["ordr_id"].toInt() == 0) {
        return;
    }
    int managerId = orderView->fields["client_id"].toInt();
    int operatorId = orderView->fields["client_operate"].toInt();

    if (Client::isOwner(managerId) ||  Client::isOwner(operatorId)){

        OrderProcessD *dialog = new OrderProcessD();
        dialog->setModel(model, orderView);
        dialog->exec();
        delete dialog;
        return;
    }

    PPLib::accessDeniedMessage();
}
