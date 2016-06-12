#include "packwidget.h"
#include "ui_packwidget.h"

PackWidget::PackWidget(QWidget *parent) :
    Order2IWidget(parent),
    ui(new Ui::PackWidget)
{
    ui->setupUi(this);
    setObjectName(QString::fromUtf8("Упаковка"));

    QString color = QString("background: rgba(255,0,255,35)");
    setStyleSheet(QString("QToolButton {%1} QFrame {%1}") .arg(color));

    on_checkRoll_toggled(true);
}

PackWidget::~PackWidget()
{
    delete ui;
}

void PackWidget::clear()
{
    ui->checkRoll->setChecked(true);
    ui->packInfo->clear();
}

void PackWidget::entity2Form(Entity *order)
{
    packType = order->fields["ordr_pack"].toInt();
    if (packType == Order::ROLL) {
        ui->checkRoll->setChecked(true);
    } else if (packType == Order::TUBE) {
        ui->checkTube->setChecked(true);
    } else {
        ui->checkOther->setChecked(true);
    }
    ui->packInfo->setPlainText(order->fields["ordr_pinfo"].toString());
}

bool PackWidget::form2Entity(Entity *order)
{
    if (isReadOnly) {
        return true;
    }
    order->fields["ordr_pinfo"] = ui->packInfo->toPlainText();
    order->fields["ordr_pack"] = packType;
    return true;
}

void PackWidget::setReadOnly()
{
    isReadOnly = true;
    ui->packFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->packInfo->setFocusPolicy(Qt::NoFocus);
    ui->checkOther->setFocusPolicy(Qt::NoFocus);
    ui->checkRoll->setFocusPolicy(Qt::NoFocus);
    ui->checkTube->setFocusPolicy(Qt::NoFocus);
}

void PackWidget::on_packInfo_textChanged()
{
    checkText(ui->packInfo->toPlainText());
}

void PackWidget::on_checkRoll_toggled(bool)
{
    packType = Order::ROLL;
    setObjectName(QString::fromUtf8("Упаковка: смотать в рулон"));
    emit nameChanged();
}

void PackWidget::on_checkTube_toggled(bool)
{
    packType = Order::TUBE;
    setObjectName(QString::fromUtf8("Упаковка: смотать на тубу"));
    emit nameChanged();
}

void PackWidget::on_checkOther_toggled(bool)
{
    packType = Order::OTHER;
    setObjectName(QString::fromUtf8("Упаковка"));
    emit nameChanged();
}
