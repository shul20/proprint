#include "paymentmodel.h"
#include "utility/money.h"
#include "entity/finance/pay.h"
#include <QDateTime>

const QStringList PaymentModel::header = QStringList()
        << QString::fromUtf8("№")
        << QString::fromUtf8("Дата")
        << QString::fromUtf8("Причина")
        << QString::fromUtf8("Относится к")
        << QString::fromUtf8("Дебет, грн.")
        << QString::fromUtf8("Кредит, грн.")
        << QString::fromUtf8("Долг, грн.")
        << QString::fromUtf8("Счет")
        << QString::fromUtf8("Внес проплату")
        << QString::fromUtf8("Инфо");


PaymentModel::PaymentModel(QObject *parent) :
    EntityModel(parent)
{
    entityHeader = &header;

}

QVariant PaymentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int column = index.column();

    if (role == Qt::TextAlignmentRole && (column == 4 || column == 5 || column == 6)) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            (column == 1 || column == 3 || column == 7)) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    int row = index.row();
    Payment *payment = static_cast<Payment *>(getEntity(row));
    //int type = payment->getPayType();

    if (role == Qt::DisplayRole && (column == 0)) {
        return payment->getId();
    }
    if (role == Qt::DisplayRole && (column == 1)) {
        return payment->fields["payment_date"].
                toDateTime().toString(PPLib::dateTimeTemplate);
    }
    if (role == Qt::DisplayRole && (column == 2)) {
        return payment->getPayName();
    }

    if (role == Qt::DisplayRole && (column == 3)) {
        return Pay::types[payment->getPayType()];
    }

    Money credit, debet;
    if (payment->getSrcSign() < 0 || payment->getDstSign() < 0) {
        credit = Money(payment->fields["payment_total"]);
        debet = 0;
    } else {
        debet = Money(payment->fields["payment_total"]);
        credit = 0;
    }


    if (role == Qt::DisplayRole && (column == 4)) {
        return debet.toString();
    }

    if (role == Qt::DisplayRole && (column == 5)) {
        return credit.toString();
    }

    if (role == Qt::DisplayRole && (column == 6)) {
        return Money(payment->fields["payment_debt"]).toString();
    }

    if (role == Qt::DisplayRole && (column == 7)) {
        QString invoice = payment->fields["payment_invoice"].toString();
        if (invoice.length() == 0) {
            return QString::fromUtf8("наличные");
        }
        return invoice;
    }

    if (role == Qt::DisplayRole && (column == 8)) {
        return payment->getClient();
    }

    if (role == Qt::DisplayRole && (column == 9)) {
        return payment->fields["payment_info"];
    }

    if (role == Qt::BackgroundColorRole && (column == 4)) {
        return QVariant(QColor("#C6FFC4"));
    }
    if (role == Qt::BackgroundColorRole && (column == 5)) {
        return QVariant(QColor("#FFC4C4"));
    }

    return EntityModel::data(index, role);
}

Payment *PaymentModel::createEntity() const
{
    return new Payment();
}
