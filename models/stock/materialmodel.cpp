#include "materialmodel.h"
#include "entity/stock/stock.h"
#include <QDate>
#include "proprintlib.h"

MaterialModel::MaterialModel(QObject *parent) :
    EntityModel(parent)
{}


QVariant MaterialModel::data(const QModelIndex &index, int role) const
{
    int column = index.column();

    if (role == Qt::TextAlignmentRole &&
            ((column == 2) || (column == 5) || (column == 6))) {
        return QVariant(Qt::AlignCenter | Qt::AlignVCenter);
    }

    if (role == Qt::TextAlignmentRole &&
            ((column == 3) || (column == 4) || (column == 7) ||
             (column == 8) || (column == 9) || (column == 10))) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);
    }

    int row = index.row();
    Material *material = static_cast<Material*>(getEntity(row));
    if (role == Qt::DisplayRole && (column == 0)) {
        return material->getProductName();
    }

    if (role == Qt::DisplayRole && (column == 1)) {
        return material->getStockName();
    }

    if (role == Qt::DisplayRole && (column == 2)) {
        if (type == Stock::ROLL){
            int width = material->getParametr();
            int length = material->fields["material_length"].toInt();
            return QString::fromUtf8("%1 х %2").arg(width).arg(length);
        }
        return material->getParametr();
    }

    if (role == Qt::DisplayRole && (column == 3)) {
        return material->fields["material_amount"];
    }

    if (role == Qt::DisplayRole && (column == 4)) {
        if (type == Stock::ROLL){
            Size size = material->fields["material_size"];
            return size.toString();
        }

        return material->fields["material_size"];
    }

    if (role == Qt::DisplayRole && (column == 5)) {
        return material->fields["material_invoice"];
    }

    if (role == Qt::DisplayRole && (column == 6)) {
        return material->fields["material_receipt"].toDate()
                .toString(PPLib::dateTemplate);
    }

    if (role == Qt::DisplayRole && (column == 7)) {
        Money price = material->fields["material_price"];
        return price.toString();
    }

    if (role == Qt::DisplayRole && (column == 8)) {
        Money payment = material->fields["material_total"];
        return payment.toString();
    }

    if (role == Qt::DisplayRole && (column == 9)) {
        Money payment = material->fields["material_payment"];
        return payment.toString();
    }

    if (role == Qt::DisplayRole && (column == 10)) {
        Money debt = material->fields["material_debt"];
        return debt.toString();
    }

    if (role == Qt::DisplayRole && (column == 11)) {
        return material->getCompanyName();
    }

    if (role == Qt::DisplayRole && (column == 12)) {
        return material->getContactName();
    }

    if (role == Qt::BackgroundColorRole && (column == 10)) {
        if (material->fields["material_debt"].toInt() > 0) {
            return QVariant(QColor("#FF9999"));
        }

        return QVariant(QColor("#99FF99"));
    }

    return EntityModel::data(index, role);
}

Material *MaterialModel::createEntity() const
{
    return new Material();
}

void MaterialModel::setStockType(int type)
{
    header = QStringList()
            << QString::fromUtf8("Продукция")
            << QString::fromUtf8("Материал");

    if (type == Stock::ROLL){
        header  << QString::fromUtf8("ШхД, мм")
                << QString::fromUtf8("Рулонов")
                << QString::fromUtf8("кв. м");

    } else if (type == Stock::LIST) {
        header  << QString::fromUtf8("H, мм")
                << QString::fromUtf8("Листов")
                << QString::fromUtf8("Всего");

    } else if (type == Stock::ITEM) {
        header  << QString::fromUtf8("Объем")
                << QString::fromUtf8("Кол-во")
                << QString::fromUtf8("Всего");
    }
        header  << QString::fromUtf8("Счет")
                << QString::fromUtf8("Поступление")
                << QString::fromUtf8("Цена")
                << QString::fromUtf8("Всего")
                << QString::fromUtf8("Оплата")
                << QString::fromUtf8("Долг")
                << QString::fromUtf8("Поставщик")
                << QString::fromUtf8("Контакт");

    entityHeader = &header;
    this->type = type;
}
