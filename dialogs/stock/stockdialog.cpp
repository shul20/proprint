#include "stockdialog.h"
#include "ui_stockdialog.h"
#include "proprintlib.h"
#include <QDebug>
#include "dialogs/productdialog.h"
#include "models/productmodel.h"
#include "models/modelmanager.h"

StockDialog::StockDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::StockDialog)
{
    ui->setupUi(this);

    productW = new ProductWidget(this);
    ui->frameProduct->layout()->addWidget(productW);
    ui->frameProduct->layout()->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding));
    productW->setup(true, false, false, false, true);

}

StockDialog::~StockDialog()
{
    delete ui;
}

void StockDialog::set(QAbstractItemModel *model, int mode,
                      const QModelIndex &index, int stockType)
{
    this->model = model;
    this->index = index;
    this->mode = mode;

    setup(stockType);
    productW->setRole(ProductModel::stockRole, stockType);

     if (mode == PPLib::addRole) {
         if (index.isValid()){
             view = (Entity *) model->data(index, PPLib::entityRole).toInt();
             entity2Form(view);
         } else {
             clear();
         }
         ui->saveStockButton->setText(QString::fromUtf8("Добавить"));
         setWindowTitle(QString::fromUtf8("Добавить позицию"));

     } else if (mode == Qt::EditRole) {
         view = (Entity *) model->data(index, PPLib::entityRole).toInt();
         entity2Form(view);
         ui->saveStockButton->setText(QString::fromUtf8("Изменить"));
         setWindowTitle(QString::fromUtf8("Редактирование продукта"));
     }
}


void StockDialog::clear()
{
    ui->name->clear();
    ui->parameter->clear();
}

void StockDialog::entity2Form(Entity *stock)
{
    ui->name->setText(stock->fields["stock_name"].toString());
    ui->parameter->setValue(stock->fields["stock_param"].toInt());

    int productId = stock->fields["product_id"].toInt();
    productW->setProduct(productId);
}

bool StockDialog::form2Entity(Entity *entity, QString &message)
{
    Stock *stock = (Stock *) entity;

    QString name = ui->name->text();
    int parametr = ui->parameter->value();

    if (name.length() == 0 || parametr == 0) {
        message = QString::fromUtf8("Заполните форму");
        return false;
    }

    stock->fields["stock_name"] = name;
    stock->fields["stock_param"] = parametr;

    Product *product = productW->getProduct();
    if (product == NULL) {
        return false;
    }

    stock->fields["product_id"] = product->fields["product_id"];
    stock->fields["stock_type"] = product->fields["product_stock"];
    stock->setProductName(product->fields["product_name"].toString());

    return true;
}

void StockDialog::setup(int type)
{    
    if (type == Stock::ROLL){
        ui->parameterName->setText(QString::fromUtf8("Ширина"));
        ui->parameterUnit->setText(QString::fromUtf8("мм"));

    } else if (type == Stock::LIST) {
        ui->parameterName->setText(QString::fromUtf8("Толщина"));
        ui->parameterUnit->setText(QString::fromUtf8("мм"));

    } else if (type == Stock::ITEM) {
        ui->parameterName->setText(QString::fromUtf8("Количество"));
        ui->parameterUnit->setText(QString::fromUtf8("л/шт."));
    }
}

void StockDialog::on_saveStockButton_clicked()
{
    Stock *saveStock = NULL;
    QString message;

    if (mode == PPLib::addRole) {
        saveStock = new Stock();

    } else if (mode == Qt::EditRole) {
        saveStock = new Stock((Stock *)view);
        saveStock->fields.remove("stock_amount");
    }

    if (!form2Entity(saveStock, message) ||
        !saveStock->update()) {

        if (message.length() == 0){
            message = Stock::lastError;
        }
        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), message);
        delete saveStock;
        return;
    }

    model->setData(index, (int) saveStock, mode);
    QDialog::accept();

}

void StockDialog::on_exitStockButton_clicked()
{
    QDialog::reject();
}
