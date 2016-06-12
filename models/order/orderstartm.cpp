#include "orderstartm.h"
#include <QDebug>
#include <QDate>
#include "entity/order/order.h"
#include "entity/entityview.h"
#include "utility/process.h"
#include "utility/size.h"

const int OrderStartM::orderIdRole = PPLib::userRole + 1;

OrderStartM::OrderStartM(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant OrderStartM::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            (column == 2 || column == 5 || column == 6 || column == 12)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            (column == 4 || column == 9)) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    int row = index.row();
    Entity *order = entities.at(row);

    if ((role == Qt::DisplayRole && (column == 0)) || (role == orderIdRole)) {
        return order->fields["ordr_id"];
    }

    if (role == Qt::DisplayRole && (column == 1)) {
        return order->fields["ordr_start"].toDateTime()
                .toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && (column == 2)) {
        int processType = order->fields["ordr_process"].toInt();
        return Process::getInstance()->getName(processType);
    }
    if (role == Qt::DisplayRole && (column == 3)) {
        return order->fields["equipment_name"];
    }
    if (role == Qt::DisplayRole && (column == 4)) {
        return order->fields["ordr_priority"];
    }
    if (role == Qt::DisplayRole && (column == 5)) {
        return order->fields["company_name"];
    }
    if (role == Qt::DisplayRole && (column == 6)) {
        return order->fields["contact_name"];
    }
    if (role == Qt::DisplayRole && (column == 7)) {
        return order->fields["product_name"];
    }
    if (role == Qt::DisplayRole && (column == 8)) {
        return order->fields["stock_name"];
    }
    if (role == Qt::DisplayRole && (column == 9)) {
        return Size(order->fields["ordr_size"]).toString();
    }
    if (role == Qt::DisplayRole && (column == 10)) {
        int status = order->fields["ordr_status"].toInt();
        return Order::statuses[status];
    }
    if (role == Qt::DisplayRole && (column == 11)) {
        return order->fields["ordr_end"].toDateTime()
                .toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && (column == 12)) {
        return order->fields["client_name"];
    }

    if (role == Qt::BackgroundRole) {
        int status = order->fields["ordr_status"].toInt();
        if (status == Order::TOWORK) {
            return QBrush(QColor("#C2F0C2"));
        }
        if (status == Order::PAUSE) {
            return QBrush(QColor("#FCCACA"));
        }
    }
    return EntityModel::data(index, role);
}

Entity *OrderStartM::createEntity() const
{
    return new EntityView();
}

const QStringList OrderStartM::header =
        QStringList() << QString::fromUtf8("№")
                      << QString::fromUtf8("Принят")
                      << QString::fromUtf8("Процесс")
                      << QString::fromUtf8("Оборудование")
                      << QString::fromUtf8("Приоритет")
                      << QString::fromUtf8("Заказчик")
                      << QString::fromUtf8("Контакт")
                      << QString::fromUtf8("Продукция")
                      << QString::fromUtf8("Материал")
                      << QString::fromUtf8("кв. м")
                      << QString::fromUtf8("Статус")
                      << QString::fromUtf8("Выполнить")
                      << QString::fromUtf8("Менеджер");
