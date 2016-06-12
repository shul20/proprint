#include "pricedialog.h"
#include "ui_pricedialog.h"
#include <QDate>
#include "models/processmodel.h"
#include "models/modelmanager.h"
#include "entity/client.h"
#include "widgets/stock/pricetypewidget.h"

PriceDialog::PriceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PriceDialog)
{
    ui->setupUi(this);

    ui->company->setup(Company::CUSTOMER, false, false, false, false);
    ui->type->setup(true);

    productW = new ProductWidget(this);
    ui->frameProduct->layout()->addWidget(productW);
    productW->setup(true, true, false, true, false);
    productW->setRole(ProductModel::processRole);

    ProcessModel *processModel = new ProcessModel(this, Process::ISPRICE);
    ui->process->setModel(processModel);

}

PriceDialog::~PriceDialog()
{
    delete ui;
}

void PriceDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
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
        ui->savePriceButton->setText(QString::fromUtf8("Добавить"));
        setWindowTitle(QString::fromUtf8("Добавить прайс"));

    } else if (mode == Qt::EditRole) {
        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);
        ui->savePriceButton->setText(QString::fromUtf8("Изменить"));
        setWindowTitle(QString::fromUtf8("Редактирование прайса"));
    }
}

void PriceDialog::entity2Form(Entity *price)
{
    int index;
    index = ui->process->findData(price->fields["price_process"], PPLib::idRole);
    ui->process->setCurrentIndex(index);
    ui->type->setType(price->fields["pricetype_id"].toInt());

    int productId = price->fields["product_id"].toInt();
    productW->setProduct(productId);

    QString stockName = price->fields["price_stock"].toString();
    if (stockName.length() != 0){
        productW->setMaterial(productId, stockName);
    }

    Money total = price->fields["price_total"];
    ui->totalPrice->setMoney(total);

    int companyId = price->fields["company_id"].toInt();
    if (companyId > 0) {
        ui->companyBox->setChecked(true);
        ui->company->setCompany(companyId);

    } else {
        ui->companyBox->setChecked(false);
    }
}

bool PriceDialog::form2Entity(Entity *entity)
{
    Price *price = (Price *) entity;

    Money total = ui->totalPrice->getMoney();
    if (total == 0) {
        PPLib::fillFormMessage();
        return false;
    }

    int processId = ui->process->itemData(ui->process->currentIndex(), PPLib::idRole).toInt();
    price->fields["price_process"] = processId;

    Product *product = productW->getProduct();
    price->fields["product_id"] = product->getId();
    price->setProduct(product->fields["product_name"].toString());

    int stock = product->fields["product_stock"].toInt();
    if (stock == Stock::NOSTOCK) {
        price->fields["price_stock"] = Stock::stocks[stock];
    } else {
        price->fields["price_stock"] = productW->getMaterial()->fields["stock_name"];
    }

    PriceType *priceType = ui->type->getPriceType();
    price->fields["pricetype_id"] = priceType->getId();
    price->setType(priceType->fields["pricetype_name"].toString());

    price->fields["client_id"] = Client::currentClient->getId();
    price->fields["price_total"] = total.cents;
    price->fields["price_date"] = QDate::currentDate();

    if (ui->companyBox->isChecked()){
        Company *company = ui->company->getCompany();
        price->fields["company_id"] = company->getId();
        price->setCompany(company->fields["company_name"].toString());
    } else {
        price->setCompany(QString::fromUtf8("Общий прайс"));
    }

    return true;
}

void PriceDialog::clear()
{
    ui->process->setCurrentIndex(0);
    ui->totalPrice->setMoney(Money(0));
    ui->companyBox->setChecked(false);
}

void PriceDialog::on_savePriceButton_clicked()
{
    Price *savePrice = NULL;

    if (mode == PPLib::addRole) {
        savePrice = new Price();

    } else if (mode == Qt::EditRole) {
        savePrice = new Price((Price *)view);
    }

    if (!form2Entity(savePrice) ||
        !savePrice->update()) {

        delete savePrice;
        return;
    }

    model->setData(index, (int) savePrice, mode);
    QDialog::accept();
/*
    if (index.isValid()){
        Price * price = (Price *)model->data(index, PPLib::entityRole).toInt();
        savePrice = new Price(price);
        if (!update(savePrice, index, Qt::EditRole)) {
            delete savePrice;
            return;
        }

    } else {

        savePrice = new Price();

        if (!update(savePrice, QModelIndex(), PPLib::addRole)) {
            delete savePrice;
            return;
        }
    }
*/
    QDialog::accept();
}

void PriceDialog::on_process_currentIndexChanged(int index)
{
    Process *process = Process::getInstance();
    int processId = ui->process->itemData(index, PPLib::idRole).toInt();

    productW->setProcess(processId);
    ui->unit->setText(process->getUnits(processId).toString());
}

void PriceDialog::on_exitPriceButton_clicked()
{
    QDialog::reject();
}
