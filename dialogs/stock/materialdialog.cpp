#include "materialdialog.h"
#include "ui_materialdialog.h"
#include "entity/stock/stock.h"
#include "proprintlib.h"
#include "entity/client.h"
#include "widgets/popupwindow.h"
#include "mainwindow.h"
#include "models/modelmanager.h"
#include "views/stock/stockview.h"


MaterialDialog::MaterialDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::MaterialDialog), stockModel(NULL)
{
    ui->setupUi(this);
    ui->company->setup(Company::SUPPLIER, true, true, true, false);

}

MaterialDialog::~MaterialDialog()
{
    delete ui;
}

void MaterialDialog::set(QAbstractItemModel *model, int mode, QModelIndex index)
{
    this->model = model;
    this->mode = mode;
    this->index = index;
    Stock *stock = NULL;
    Material *material = NULL;

    if (mode == PPLib::addRole){
      stockModel = (StockModel *)index.model();
      stock = (Stock *) stockModel->data(index, PPLib::entityRole).toInt();

    } else if (mode == Qt::EditRole){
        material = (Material *) model->data(index, PPLib::entityRole).toInt();
        stockModel = (StockModel *) ModelManager::getInstance()->getStockModel();
        stock = (Stock *)stockModel->findEntity(material->fields["stock_id"].toInt());
        if (stock == NULL) {
            return;
        }


    }

    setup(stock);
    entity2Form(stock, material);
    setPermission(mode);
}

void MaterialDialog::on_saveMaterialButton_clicked()
{
    int currentStock = StockView::instance->currentMaterialStock();
    Material *saveMaterial = NULL;
    QString message;

    if (mode == PPLib::addRole){
        saveMaterial = new Material();

    } else if (mode == Qt::EditRole){
        Material *material = (Material *) model->data(index, PPLib::entityRole).toInt();
        Money pay = Money(material->fields["material_payment"]);
        if (pay > 0){
            QMessageBox::critical(this, QString::fromUtf8("Невозможно изменить материал"),
                                  QString::fromUtf8("За материал внесена оплата."));
            return;
        }
        saveMaterial = new Material(material);
    }

    MainWindow::db->transaction();
    if (!form2Entity(saveMaterial, message) ||
        !saveMaterial->update()) {

        MainWindow::db->rollback();
        if (message.length() == 0) {
            message = Material::lastError;
        }

        delete saveMaterial;

        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), message);
        return;
    }

    MainWindow::db->commit();

    int stockType = saveMaterial->getStockType();
    if (stockType == currentStock) {
        model->setData(index, (int) saveMaterial, mode);
    }

    retriveContact();
    ui->company->updateInfo();
    retriveStock();

    PopUpWindow *popMessage = new PopUpWindow(this, QString::fromUtf8("материал учтен"));
    popMessage->show();
    QTimer::singleShot(700, popMessage, SLOT(deleteLater()));
}

bool MaterialDialog::form2Entity(Entity *entity, QString &message)
{
    QString invoice = ui->invoice->getInvoice();
    if (size == 0 || invoice.length() == 0) {
        message = QString::fromUtf8("Заполните форму");
        return false;
    }
    Material *material = (Material *) entity;
    Money total = ui->total->getMoney();
    material->fields["material_total"] = total.cents;
    material->fields["material_debt"] = total.cents;
    material->fields["material_price"] = ui->price->getMoney().cents;
    material->fields["material_invoice"] = invoice;
    material->fields["material_amount"] = amountBox->value();
    material->fields["material_size"] = size;
    material->fields["material_length"] = ui->length->value();
    material->fields["material_receipt"] = ui->dateReceipt->date();

    if (mode != PPLib::addRole) {
        return true;
    }

    Stock *stock = productWidget->getParam();
    material->fields["stock_id"] = stock->getId();
    material->setProductName(stock->getProductName());
    material->setStockName(stock->fields["stock_name"].toString());
    material->setStockType(stock->fields["stock_type"].toInt());

    Company * company = ui->company->getCompany();
    material->setCompanyId(company->getId());
    material->setCompanyName(company->fields["company_name"].toString());

    Contact *contact = ui->company->getContact();
    material->fields["contact_id"] = contact->getId();
    material->setContactName(contact->fields["contact_name"].toString());

    material->setParametr(stock->fields["stock_param"].toInt());

    return true;
}

void MaterialDialog::setPermission(int mode)
{
    if (mode == Qt::EditRole) {
        productWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
        productWidget->setFocusPolicy(Qt::NoFocus);

        ui->company->setAttribute(Qt::WA_TransparentForMouseEvents);
        ui->company->setFocusPolicy(Qt::NoFocus);

        if (Client::currentRole == Client::ADMIN){
            ui->saveMaterialButton->setText(QString::fromUtf8("Изменить"));
            setWindowTitle(QString::fromUtf8("Редактирование"));
            return;
        }
        ui->mainBox->setAttribute(Qt::WA_TransparentForMouseEvents);
        ui->saveMaterialButton->hide();
    }
}

void MaterialDialog::setup(Stock *stock)
{
    int stockType = stock->fields["stock_type"].toInt();
    if (stockType == Stock::ROLL){
        ui->listFrame->hide();
        ui->itemFrame->hide();
        productWidget = ui->productRoll;

        amountBox = ui->rollAmount;
        sizeBox = ui->rollSize;

    } else if (stockType == Stock::LIST){
        ui->rollFrame->hide();
        ui->itemFrame->hide();
        productWidget = ui->productList;
        amountBox = ui->listAmount;

    } else if (stockType == Stock::ITEM){
        ui->rollFrame->hide();
        ui->listFrame->hide();
        productWidget = ui->productItem;
        amountBox = ui->itemAmount;
        sizeBox = ui->itemSize;
    }

    connect(productWidget, SIGNAL(paramChanged(Stock*)), this, SLOT(stockChanged(Stock *)));
    connect(amountBox, SIGNAL(valueChanged(int)), this, SLOT(amountChanged(int)));

    productWidget->setup(true, true, true, false, false);
    productWidget->setRole(ProductModel::stockRole, stock->fields["stock_type"]);


    adjustSize();


    ui->dateReceipt->setDate(QDate::currentDate());
}

void MaterialDialog::entity2Form(Stock *stock, Material *material)
{
    int productId = stock->fields["product_id"].toInt();
    productWidget->setParam(productId, stock->getId());

    if (mode == Qt::EditRole){
        ui->length->setValue(material->fields["material_length"].toInt());
        amountBox->setValue(material->fields["material_amount"].toInt());

        QString invoice = material->fields["material_invoice"].toString();
        ui->invoice->setInvoice(invoice);
        ui->dateReceipt->setDate(material->fields["material_receipt"].toDate());
        ui->price->setMoney(Money(material->fields["material_price"]));

        int companyId = material->getCompanyId();
        int contactId = material->fields["contact_id"].toInt();
        ui->company->setContact(companyId, contactId);
    }
}

void MaterialDialog::amountChanged(int)
{
    calculateTotal();
    calculateSize();
}

void MaterialDialog::stockChanged(Stock *)
{
    calculateSize();
}

void MaterialDialog::calculateTotal()
{
    Money price = ui->price->getMoney();
    int amount = amountBox->value();

    Money total = price * (amount * 100);
    ui->total->setMoney(total);
}

void MaterialDialog::calculateSize()
{
    int amount = amountBox->value();
    Stock *stock = productWidget->getParam();
    if (stock == NULL) {
        sizeBox->clear();
        return;
    }

    int parametr = stock->fields["stock_param"].toInt();
    int stockType = stock->fields["stock_type"].toInt();

    if (stockType == Stock::ROLL){
        int length = ui->length->value();
        Size rollSize = Size(parametr, length) * amount;

        sizeBox->setText(rollSize.toString());
        size = rollSize.value;

    } else if (stockType == Stock::LIST){
        size = amount;

    } else if (stockType == Stock::ITEM){
        size = amount * parametr;
        sizeBox->setText(QString::number(size));
    }
}

void MaterialDialog::on_price_textChanged(const QString &arg1)
{
    calculateTotal();
}

void MaterialDialog::on_length_valueChanged(int arg1)
{
    calculateSize();
}

void MaterialDialog::on_exitMaterialButton_clicked()
{
    QDialog::reject();
}

void MaterialDialog::retriveContact()
{
    int contactId = ui->company->getContact()->getId();

    Entity *contact = ModelManager::getInstance()->getContactModel()->findEntity(contactId);
    int companyId = contact->fields["company_id"].toInt();
    Entity *company = ModelManager::getInstance()->getCompanyModel()->findEntity(companyId);

    contact->retrive(contactId, false);
    company->retrive(companyId, false);
}

void MaterialDialog::retriveStock()
{
    Entity *stock = productWidget->getParam();

    if (stock == NULL) {
        return;
    }
    stock->retrive(stock->getId(), false);
}
