#include "pricetypewidget.h"
#include "ui_pricetypewidget.h"
#include "models/modelmanager.h"
#include <QLineEdit>

PriceTypeWidget::PriceTypeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PriceTypeWidget)
{
    ui->setupUi(this);

    ModelManager *models = ModelManager::getInstance();

    ui->type->setModel(models->getPriceTypeModel());
    ui->type->setEditable(true);
}

PriceTypeWidget::~PriceTypeWidget()
{
    delete ui;
}

void PriceTypeWidget::setup(bool isButton)
{
    ui->buttonFrame->setHidden(!isButton);
}

void PriceTypeWidget::setType(int priceTypeId)
{
    int index = ui->type->findData(priceTypeId, PPLib::idRole);
    if (index < 0) {
        PriceType *priceType = new PriceType();
        if (!priceType->retrive(priceTypeId, false)){
            delete priceType;
            return;
        }
        ui->type->model()->setData(QModelIndex(), (int)priceType, PPLib::addRole);
        ui->type->setCurrentIndex(ui->type->count() - 1);
        return;
    }
    ui->type->setCurrentIndex(index);
}

PriceType *PriceTypeWidget::getPriceType()
{
    int index = ui->type->currentIndex();
    return (PriceType *) ui->type->itemData(index, PPLib::entityRole).toInt();
}

void PriceTypeWidget::on_type_currentIndexChanged(int index)
{
    PriceType *priceType = (PriceType *) ui->type->itemData(index, PPLib::entityRole).toInt();
    emit priceTypeChanged(priceType);
}

void PriceTypeWidget::on_addPriceType_clicked()
{
    QString text = ui->type->lineEdit()->text();
    if (text.length() == 0){
        return;
    }
    PriceType *priceType = new PriceType();
    priceType->fields["pricetype_name"] = text;
    if (!priceType->update()) {
        return;
    }
    QAbstractItemModel *model = ui->type->model();
    model->setData(QModelIndex(), (int)priceType, PPLib::addRole);
    ui->type->setCurrentIndex(model->rowCount() - 1);
}

void PriceTypeWidget::on_delPriceType_clicked()
{
    int result = QMessageBox::question(this, QString::fromUtf8("Удаление"),
               QString::fromUtf8("Удалить выбранный тип?"), QMessageBox::Cancel | QMessageBox::Ok);

    if (result == QMessageBox::Cancel) {
        return;
    }
    QAbstractItemModel *model = ui->type->model();

    QModelIndex index = model->index(ui->type->currentIndex(), 0);
    model->setData(index, true, PPLib::delRole);
    ui->type->setCurrentIndex(0);
}
