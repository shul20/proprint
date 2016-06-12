#include "productwidget.h"
#include "ui_productwidget.h"
#include "models/modelmanager.h"
#include "utility/process.h"
#include "models/uniquefilter.h"
#include "dialogs/productdialog.h"

ProductWidget::ProductWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWidget), materialF(NULL)
{
    ui->setupUi(this);

    ProductModel *productModel = ModelManager::getInstance()->getProductModel();
    productF = new SortFilterModel(this);

    productF->setSourceModel(productModel);
    connect(productModel, SIGNAL(modelReset()), this, SLOT(productModelReset()));

}

ProductWidget::~ProductWidget()
{
    delete ui;
}

void ProductWidget::setup(bool isProduct, bool isMaterial, bool isParam,
                          bool isMaterialLabel, bool isButton)
{
    ui->product->setHidden(!isProduct);
    ui->frameMaterial->setHidden(!isMaterial);
    ui->param->setHidden(!isParam);
    ui->labelStock->setHidden(!isMaterialLabel);
    ui->addProduct->setHidden(!isButton);

    if (!isMaterial) {
        return;
    }

    StockModel *stockModel = ModelManager::getInstance()->getStockModel();
    stockF = new SortFilterModel(this);

    stockF->setSourceModel(stockModel);
    stockF->setFilterRole(StockModel::productIdRole);

    connect(stockModel, SIGNAL(modelReset()), this, SLOT(stockModelReset()));

    if (!isParam) {
        return;
    }

    paramF = new SortFilterModel(this);
    paramF->setSourceModel(stockF);
    paramF->setFilterRole(PPLib::entityRole);
    paramF->compare = &SortFilterModel::paramFilter;
    ui->param->setModel(paramF);
    ui->param->setModelColumn(StockModel::PARAMETR);

}

void ProductWidget::setRole(int role, QVariant match)
{
    if (role == ProductModel::processRole) {
        productF->compare = &PPLib::checkBit;
    }
    productF->setFilterRole(role);
    productF->setMatchValue(match);
    ui->product->setModel(productF);
}

void ProductWidget::on_product_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }

    Product *product = (Product *) ui->product->itemData(index, PPLib::entityRole).toInt();
    int stockType = product->fields["product_stock"].toInt();

    if (stockType == Stock::NOSTOCK){
        ui->frameMaterial->setDisabled(true);
        return;

    } else {
        ui->frameMaterial->setEnabled(true);
    }

    if (!ui->frameMaterial->isHidden()) {
        stockF->setMatchValue(product->getId());
        if (materialF != NULL) {
            delete materialF;
        }

        materialF = new UniqueFilter(this);
        materialF->setSourceModel(stockF);
        materialF->setFilterRole(PPLib::uniqueRole);
        ui->material->setModel(materialF);
        ui->material->setModelColumn(StockModel::NAME);
    }

    emit productChanged(product);
    emit stockChanged();
}

void ProductWidget::on_material_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }
    Stock *stock = (Stock *) ui->material->itemData(index, PPLib::entityRole).toInt();

    if (!ui->param->isHidden()) {
        paramF->setMatchValue(int(stock));
    }

    emit materialChanged(stock);
    emit stockChanged();
}

void ProductWidget::on_param_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }
    Stock *stock = (Stock *) ui->param->itemData(index, PPLib::entityRole).toInt();
    emit paramChanged(stock);
    emit stockChanged();
}


void ProductWidget::setProduct(int productId)
{
    int index = ui->product->findData(productId, PPLib::idRole);
    if (index < 0) {
        QMessageBox::information(this, QString::fromUtf8("Продукция"),
                                 QString::fromUtf8("Id не найден"));

        Product *product = new Product();
        if (!product->retrive(productId, false)){
            delete product;
            return;
        }
        productF->setData(QModelIndex(), (int)product, PPLib::addRole);
        ui->product->setCurrentIndex(ui->product->count() - 1);
        return;
    }
    ui->product->setCurrentIndex(index);
}

Product *ProductWidget::getProduct() const
{
    int index = ui->product->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Product *) ui->product->itemData(index, PPLib::entityRole).toInt();
}

Stock *ProductWidget::getMaterial() const
{
    int index = ui->material->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Stock *) ui->material->itemData(index, PPLib::entityRole).toInt();
}

void ProductWidget::setMaterial(int productId, int stockId)
{
    setProduct(productId);
    QModelIndexList indexes = stockF->match(stockF->index(0,0), PPLib::idRole, stockId);
    Stock *stock;

    if (indexes.size() == 0) {
        stock = new Stock();
        if (!stock->retrive(stockId, false)){
            delete stock;
            return;
        }
        stockF->setData(QModelIndex(), (int)stock, PPLib::addRole);

    } else {
        stock = (Stock *) stockF->data(indexes.first(), PPLib::entityRole).toInt();
    }

    int index = ui->material->findText(stock->fields["stock_name"].toString());
    ui->material->setCurrentIndex(index);
}

void ProductWidget::setMaterial(int productId, const QString name)
{
    setProduct(productId);
    int index = ui->material->findText(name);
    ui->material->setCurrentIndex(index);
}

Stock *ProductWidget::getParam() const
{
    int index = ui->param->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Stock *) ui->param->itemData(index, PPLib::entityRole).toInt();
}

void ProductWidget::setParam(int productId, int stockId)
{
    setMaterial(productId, stockId);
    int index = ui->param->findData(stockId, PPLib::idRole);
    ui->param->setCurrentIndex(index);
}

void ProductWidget::setProcess(int processId)
{
    productF->setMatchValue(processId);
}

void ProductWidget::on_addProduct_clicked()
{
    int row = ui->product->currentIndex();
    QModelIndex index = productF->index(row, 0);

    ProductDialog *dialog = new ProductDialog(this);
    dialog->set(productF, PPLib::addRole, index);

    if (dialog->exec() == QDialog::Accepted){
        ui->product->setCurrentIndex(productF->rowCount() - 1);
    }
    delete dialog;
}

void ProductWidget::productModelReset()
{
    ui->product->setModel(productF);
}

void ProductWidget::stockModelReset()
{
    ui->material->setModel(materialF);

    if (!ui->param->isHidden()) {
        ui->param->setModel(paramF);
    }
}

void ProductWidget::disableFocus()
{
    ui->product->setFocusPolicy(Qt::NoFocus);
    ui->material->setFocusPolicy(Qt::NoFocus);
    ui->param->setFocusPolicy(Qt::NoFocus);
}
