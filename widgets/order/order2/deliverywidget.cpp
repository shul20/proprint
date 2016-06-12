#include "deliverywidget.h"
#include "ui_deliverywidget.h"

DeliveryWidget::DeliveryWidget(QWidget *parent) :
    Order2IWidget(parent),
    ui(new Ui::DeliveryWidget)
{
    ui->setupUi(this);
    setObjectName(QString::fromUtf8("Доставка"));

    QString color = QString("background: rgba(0,85,127,65)");
    setStyleSheet(QString("QToolButton {%1} QFrame {%1}") .arg(color));

    for (int i = 0; i < Order::deliveries.length(); i++){
        ui->deliveryType->addItem(Order::deliveries[i], i);
    }
}

DeliveryWidget::~DeliveryWidget()
{
    delete ui;
}

void DeliveryWidget::clear()
{
    ui->deliveryType->setCurrentIndex(0);
    ui->deliveryInfo->clear();
}

void DeliveryWidget::entity2Form(Entity *order)
{
    ui->deliveryType->setCurrentIndex(order->fields["ordr_delivery"].toInt());
    ui->deliveryInfo->setPlainText(order->fields["ordr_dinfo"].toString());
    QString declaration = order->fields["ordr_declaration"].toString();
    if (declaration.length() != 0) {
        ui->deliveryDeclaration->setText(declaration);
    }
}

bool DeliveryWidget::form2Entity(Entity *order)
{
    if (isReadOnly) {
        return true;
    }
    order->fields["ordr_delivery"] = ui->deliveryType->currentIndex();
    order->fields["ordr_declaration"] = ui->deliveryDeclaration->text();
    order->fields["ordr_dinfo"] = ui->deliveryInfo->toPlainText();
    return true;
}

void DeliveryWidget::setReadOnly()
{
    isReadOnly = true;
    ui->deliveryFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->deliveryInfo->setFocusPolicy(Qt::NoFocus);
    ui->deliveryType->setFocusPolicy(Qt::NoFocus);
    ui->deliveryDeclaration->setFocusPolicy(Qt::NoFocus);
}

void DeliveryWidget::on_deliveryType_currentIndexChanged(int index)
{
    setObjectName(QString::fromUtf8("Доставка: %1").
                                arg(Order::deliveries[index]));
    if (index == Order::NEWPOST) {
        ui->deliveryDeclaration->setEnabled(true);
        emit nameChanged();
        return;
    }
    ui->deliveryDeclaration->setText(QString::fromUtf8("нет"));
    ui->deliveryDeclaration->setDisabled(true);
    emit nameChanged();
}

void DeliveryWidget::on_deliveryInfo_textChanged()
{
    checkText(ui->deliveryInfo->toPlainText());
}
