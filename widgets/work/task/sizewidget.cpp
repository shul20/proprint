#include "sizewidget.h"
#include "ui_sizewidget.h"
#include "entity/order/work.h"

SizeWidget::SizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SizeWidget), width(0), height(0), amount(0), workSize(0)
{
    ui->setupUi(this);
}

SizeWidget::~SizeWidget()
{
    delete ui;
}

void SizeWidget::on_width_valueChanged(int arg1)
{
    width = arg1;
    calculateSize();
}

void SizeWidget::on_height_valueChanged(int arg1)
{
    height = arg1;
    calculateSize();
}

void SizeWidget::on_amount_valueChanged(int arg1)
{
    amount = arg1;
    calculateSize();
}

void SizeWidget::calculateSize()
{
    if (width < height) {
        workSize = Size(width * amount, height);
    } else {
        workSize = Size(width, height * amount);
    }

    ui->size->setText(workSize.toString());
    emit sizeChanged();
}

void SizeWidget::clear(){
    ui->width->setValue(0);
    ui->height->setValue(0);
    ui->amount->setValue(0);
}

void SizeWidget::entity2Form(Entity *entity)
{
    ui->width->setValue(entity->fields["work_width"].toInt());
    ui->height->setValue(entity->fields["work_height"].toInt());
    ui->amount->setValue(entity->fields["work_amount"].toInt());
}

bool SizeWidget::form2Entity(Entity *entity)
{
    int size = workSize.value;

    if (size == 0) {
        return false;
    }

    entity->fields["work_width"] = ui->width->value();
    entity->fields["work_height"] = ui->height->value();
    entity->fields["work_amount"] = ui->amount->value();
    entity->fields["work_size"] = size;

    return true;
}

int SizeWidget::getAmount()
{
    return ui->amount->value();
}

int SizeWidget::getWidth()
{
    return ui->width->value();
}

int SizeWidget::getHeight()
{
    return ui->height->value();
}

Size &SizeWidget::getSize()
{
    return workSize;
}
