#include "paydialog.h"
#include "ui_paydialog.h"
#include "models/modelmanager.h"
#include "models/finance/accountmodel.h"
#include <QDebug>
#include <QColorDialog>
#include "entity/client.h"

PayDialog::PayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayDialog)
{
    ui->setupUi(this);

    AccountModel *accountModel = ModelManager::getInstance()->getAccountModel();

    ui->srcAcount->setModel(accountModel);
    ui->dstAcount->setModel(accountModel);

    for (int i = 0; i < Pay::types.size(); i++){
        ui->typeBox->addItem(Pay::types[i]);
    }

    for (int i = 0; i < Pay::credits.size(); i++){
        ui->srcCreditBox->addItem(Pay::credits[i]);
        ui->dstCreditBox->addItem(Pay::credits[i]);
    }

    for (int i = 0; i < Pay::deposits.size(); i++){
        ui->debtBox->addItem(Pay::deposits[i]);
    }

    ui->permissionBox->addItem(Client::roles[Client::ADMIN]);
    ui->permissionBox->addItem(Client::roles[Client::MANAGER]);

    ui->permissionBox->setCurrentIndex(Client::MANAGER);

}

PayDialog::~PayDialog()
{
    delete ui;
}

void PayDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
{
    this->model = model;
    this->index = index;
    this->mode = mode;

    if (mode == PPLib::addRole) {
        if (index.isValid()){
            view = (Entity *) model->data(index, PPLib::entityRole).toInt();
            entity2Form(view);
        } else {
            clear();
        }
        ui->saveButton->setText(QString::fromUtf8("Создать"));
        setWindowTitle(QString::fromUtf8("Создать платеж"));

    } else if (mode == Qt::EditRole) {
        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);
        ui->saveButton->setText(QString::fromUtf8("Изменить"));
        setWindowTitle(QString::fromUtf8("Редактирование платежа"));
    }
}

void PayDialog::on_exitButton_clicked()
{
    QDialog::reject();
}

void PayDialog::on_saveButton_clicked()
{
    Pay *savePay = NULL;

    if (mode == PPLib::addRole) {
        savePay = new Pay();

    } else if (mode == Qt::EditRole) {
        savePay = new Pay((Pay *)view);
    }

    if (!form2Entity(savePay) ||
        !savePay->update()) {

        delete savePay;
        return;
    }

    model->setData(index, (int) savePay, mode);
    QDialog::accept();

}

bool PayDialog::form2Entity(Entity *pay)
{
    QString name = ui->name->text();

    if (name.length() == 0) {
        PPLib::fillFormMessage();
        return false;
    }

    pay->fields["pay_name"] = name;
    pay->fields["pay_type"] = ui->typeBox->currentIndex();
    pay->fields["pay_debt"] = ui->debtBox->currentIndex();
    pay->fields["pay_permission"] = ui->permissionBox->currentIndex();

    if (ui->srcBox->isChecked()){
        int index = ui->srcAcount->currentIndex();
        pay->fields["src_account"] = ui->dstAcount->itemData(index, PPLib::idRole).toInt();
        pay->fields["src_sign"] = getSign(ui->srcCreditBox);
    } else {
        pay->fields["src_account"] = QVariant(QVariant::String);
    }

    if (ui->dstBox->isChecked()){
        int index = ui->dstAcount->currentIndex();
        pay->fields["dst_account"] = ui->dstAcount->itemData(index, PPLib::idRole).toInt();
        pay->fields["dst_sign"] = getSign(ui->dstCreditBox);
    } else {
        pay->fields["dst_account"] = QVariant(QVariant::String);
    }

    int r,g,b;
    ui->frame->palette().color(QPalette::Background).getRgb(&r, &g, &b);
    pay->fields["pay_color"] = QString("#%1%2%3").arg(r, 2, 16)
                                                 .arg(g, 2, 16)
                                                 .arg(b, 2, 16);
    return true;
}

void PayDialog::entity2Form(Entity *pay)
{
    ui->name->setText(pay->fields["pay_name"].toString());
    ui->typeBox->setCurrentIndex(pay->fields["pay_type"].toInt());
    ui->debtBox->setCurrentIndex(pay->fields["pay_debt"].toInt());
    ui->permissionBox->setCurrentIndex(pay->fields["pay_permission"].toInt());

    int srcAccountId = pay->fields["src_account"].toInt();
    if (srcAccountId > 0){
        ui->srcBox->setChecked(true);
        int index = ui->srcAcount->findData(srcAccountId, PPLib::idRole);
        ui->srcAcount->setCurrentIndex(index);
        setSign(ui->srcCreditBox, pay->fields["src_sign"].toInt());

    } else {
        ui->srcBox->setChecked(false);
    }

    int dstAccountId = pay->fields["dst_account"].toInt();
    if (dstAccountId > 0){
        ui->dstBox->setChecked(true);
        int index = ui->dstAcount->findData(dstAccountId, PPLib::idRole);
        ui->dstAcount->setCurrentIndex(index);
        setSign(ui->dstCreditBox, pay->fields["dst_sign"].toInt());

    } else {
        ui->dstBox->setChecked(false);
    }

    QColor color(pay->fields["pay_color"].toString());
    paintButton(color);
}

void PayDialog::clear()
{
}

void PayDialog::paintButton(QColor &color)
{
    QPalette pal = ui->frame->palette();
    pal.setColor(QPalette::Background, color);
    ui->frame->setPalette(pal);
    ui->frame->setAutoFillBackground( true );
}

int PayDialog::getSign(QComboBox *box)
{
    int creditIndex = box->currentIndex();
    if (creditIndex == Pay::CREDIT) {
        return -1;

    } else if (creditIndex == Pay::DEBET) {
        return 1;
    }

    return 0;
}

void PayDialog::setSign(QComboBox *box, int sign)
{
    if (sign < 0) {
        box->setCurrentIndex(Pay::CREDIT);
    } else {
        box->setCurrentIndex(Pay::DEBET);
    }
}

void PayDialog::on_colorButton_clicked()
{
    QColorDialog dialog;
    QColor color = dialog.getColor();
    paintButton(color);
}
