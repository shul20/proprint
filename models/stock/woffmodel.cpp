#include "woffmodel.h"
#include "entity/stock/stock.h"
#include <QDate>
#include "proprintlib.h"
#include "utility/process.h"

QString showInfo(Woff *woff);

WoffModel::WoffModel(QObject *parent) :
    EntityModel(parent)
{
}

QVariant WoffModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();
    if (type == Stock::ROLL){
        if (role == Qt::TextAlignmentRole &&
                (column == 1 || column == 4 || column == 9)) {
            return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
        }

        if (role == Qt::TextAlignmentRole &&
                ((column == 5) ||
                 (column == 6) || (column == 7) || (column == 8))) {
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        }

    } else {
        if (role == Qt::TextAlignmentRole &&
                (column == 1 || column == 4 || column == 6)) {
            return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
        }

        if (role == Qt::TextAlignmentRole && column == 5) {
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        }
    }

    int row = index.row();
    Woff *woff = static_cast<Woff*>(getEntity(row));
    if (role == Qt::DisplayRole && (column == 0)) {
        return woff->getId();
    }
    if (role == Qt::DisplayRole && (column == 1)) {
        return woff->fields["woff_date"].toDateTime()
                .toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && (column == 2)) {
        return woff->getProduct();
    }

    if (role == Qt::DisplayRole && (column == 3)) {
        return woff->getStock();
    }
    int param = woff->getStockParam().toInt();
    if (role == Qt::DisplayRole && (column == 4)) {
        return param;
    }
    if (role == Qt::DisplayRole && (column == 5)) {
        int amount = woff->fields["woff_amount"].toInt();

        if (type == Stock::ROLL){
             return Size(amount).toString();
        }
        if (type == Stock::ITEM){
             return amount / param;
        }
        return amount;
    }
    if (type == Stock::ROLL){
        if (role == Qt::DisplayRole && (column == 6)) {
            return woff->getOrderSize().toString();
        }
        if (role == Qt::DisplayRole && (column == 7)) {
            return Size(woff->fields["woff_waste"]).toString();
        }
        if (role == Qt::DisplayRole && (column == 8)) {
            return Size(woff->fields["woff_over"]).toString();
        }
        if (role == Qt::DisplayRole && (column == 9)) {
            return woff->getClient();
        }
        if (role == Qt::DisplayRole && (column == 10)) {
            return showInfo(woff);
        }

    }

    if (role == Qt::DisplayRole && (column == 6)) {
        return woff->getClient();
    }
    if (role == Qt::DisplayRole && (column == 7)) {
        return showInfo(woff);
    }

    if (role == Qt::BackgroundRole) {
        int over = woff->fields["woff_over"].toInt();
        if (over > 0) {
            return QBrush(QColor("#FFB2B2"));
        }
    }

    return EntityModel::data(index, role);
}

Woff *WoffModel::createEntity() const
{
    return new Woff();
}

void WoffModel::setStockType(int type)
{
    header = QStringList()
            << QString::fromUtf8("№")
            << QString::fromUtf8("Дата")
            << QString::fromUtf8("Продукция")
            << QString::fromUtf8("Материал");

    if (type == Stock::ROLL){
        header  << QString::fromUtf8("Ш, мм")
                << QString::fromUtf8("Списано кв.м")
                << QString::fromUtf8("Заказ кв.м")
                << QString::fromUtf8("Брак")
                << QString::fromUtf8("Сверх");

    } else if (type == Stock::LIST) {
        header  << QString::fromUtf8("H, мм")
                << QString::fromUtf8("Листов");

    } else if (type == Stock::ITEM) {
        header  << QString::fromUtf8("Объем")
                << QString::fromUtf8("Кол-во");
    }

    header  << QString::fromUtf8("Списал")
            << QString::fromUtf8("Инфо");

    entityHeader = &header;
    this->type = type;
}


QString showInfo(Woff *woff)
{
    int orderId = woff->fields["ordr_id"].toInt();
    int paymentId = woff->fields["payment_id"].toInt();
    QString info;

    if (orderId > 0) {
        int task = woff->fields["task_process"].toInt();
        info = QString::fromUtf8("Заказ №%1 (%2). ")
                .arg(orderId)
                .arg(Process::getInstance()->getName(task).toString());

    } else if (paymentId > 0) {
        info = QString::fromUtf8("Продажа (платеж № %1). ")
                .arg(paymentId);
    } else {
        info = QString::fromUtf8("В убыток. ");
    }

    return info.append(woff->fields["woff_info"].toString());
}
