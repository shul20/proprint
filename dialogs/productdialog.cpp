#include "productdialog.h"
#include "ui_productdialog.h"
#include "entity/stock/stock.h"
#include "proprintlib.h"
#include <QDebug>

ProductDialog::ProductDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::ProductDialog)
{
    ui->setupUi(this);

    for (int i = 0; i < Stock::stocks.length(); i++){
            ui->stock->addItem(Stock::stocks[i], i);
    }

    processModel = new ProcessModel(this, Process::ISPRODUCT);
    processModel->setCheck(true);
    ui->processes->setModel(processModel);
    ui->processes->setStyleSheet("QListView::item { padding: 3px; }");
}

ProductDialog::~ProductDialog()
{
    delete ui;
}

void ProductDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
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
        ui->saveProductButton->setText(QString::fromUtf8("Добавить"));
        setWindowTitle(QString::fromUtf8("Добавить продукт"));

    } else if (mode == Qt::EditRole) {
        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);
        ui->saveProductButton->setText(QString::fromUtf8("Изменить"));
        setWindowTitle(QString::fromUtf8("Редактирование продукта"));
    }
}


void ProductDialog::on_saveProductButton_clicked()
{
    Product *saveProduct = NULL;

    if (mode == PPLib::addRole) {
        saveProduct = new Product();

    } else if (mode == Qt::EditRole) {
        saveProduct = new Product((Product *)view);
    }

    if (!form2Entity(saveProduct) ||
        !saveProduct->update()) {

        delete saveProduct;
        return;
    }

    model->setData(index, (int) saveProduct, mode);
    QDialog::accept();
}


bool ProductDialog::form2Entity(Entity *entity)
{
    Product *product = (Product *) entity;
    const QString name = ui->name->text();

    if (name.length() == 0) {
        PPLib::fillFormMessage();
        return false;
    }

    product->fields["product_name"] = name;
    product->fields["product_stock"] = ui->stock->currentIndex();
    product->setProcesses(processModel->getProcesses());

    return true;
}

void ProductDialog::entity2Form(Entity *product)
{
    int stockType = product->fields["product_stock"].toInt();
    ui->stock->setCurrentIndex(stockType);

    ui->name->setText(product->fields["product_name"].toString());
    processModel->setProcesses(product->fields["product_processes"].toInt());
}

void ProductDialog::clear()
{
    ui->name->clear();
    ui->stock->setCurrentIndex(0);
    processModel->setProcesses(0);
}

void ProductDialog::on_exitProductButton_clicked()
{
    QDialog::reject();
}
