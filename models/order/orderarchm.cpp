#include "orderarchm.h"
#include <QDebug>
#include <QDate>
#include "entity/order/order.h"
#include "entity/entityview.h"
#include "utility/process.h"
#include "utility/size.h"

OrderArchM::OrderArchM(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;
}

QVariant OrderArchM::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            ((column == 2) || (column == 5) || (column == 6))) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            ((column == 3) || (column == 4) || (column == 7) ||
             (column == 8) || (column == 9))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    int row = index.row();
    Entity *order = entities.at(row);

    if (role == Qt::DisplayRole && (column == 0)) {
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
        return order->fields["ordr_release"].toDateTime()
                .toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && (column == 4)) {
        return order->fields["equipment_name"];
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
        return order->fields["client_name"];
    }
    if (role == Qt::DisplayRole && (column == 11)) {
        return order->fields["ordr_story"];
    }

    if (role == Qt::BackgroundRole) {
        if (order->fields["ordr_debt"].toInt() > 0 ) {
            return QBrush(QColor("#FFDEDE"));
        }
    }
    return EntityModel::data(index, role);
}

EntityView *OrderArchM::createEntity() const
{
    return new EntityView();
}

const QStringList OrderArchM::header =
        QStringList() << QString::fromUtf8("№")
                      << QString::fromUtf8("Принят")
                      << QString::fromUtf8("Процесс")
                      << QString::fromUtf8("Отдан")
                      << QString::fromUtf8("Оборудование")
                      << QString::fromUtf8("Заказчик")
                      << QString::fromUtf8("Контакт")
                      << QString::fromUtf8("Продукция")
                      << QString::fromUtf8("Материал")
                      << QString::fromUtf8("кв. м")
                      << QString::fromUtf8("Менеджер")
                      << QString::fromUtf8("Сюжет");

