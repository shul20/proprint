#include "cutrolldialog.h"
#include "ui_cutrolldialog.h"
#include <QDebug>
#include "utility/size.h"
#include "mainwindow.h"
#include "models/modelmanager.h"

CutRollDialog::CutRollDialog(QAbstractItemModel *model, Stock *stock) :
    ui(new Ui::CutRollDialog), stock(stock)
{
    ui->setupUi(this);

    entity2Form(stock);
    cutWidthModel = new IntListModel(this);
    ui->newRolls->setModel(cutWidthModel);

    widthModel = new SortFilterModel(this);
    widthModel->setSourceModel(model);
    widthModel->setFilterRole(PPLib::entityRole);
    widthModel->compare = &SortFilterModel::paramFilter;
    widthModel->setMatchValue(int(stock));


    ui->rolls->setModel(widthModel);
    ui->rolls->setModelColumn(StockModel::PARAMETR);
}

CutRollDialog::~CutRollDialog()
{
    delete ui;
}

void CutRollDialog::on_cutButton_clicked()
{
    if (width == 0) {
        return;
    }
    width -= cutWidth;
    cutWidthModel->appendInt(cutWidth);
    ui->cutWidth->setValue(width);
}

void CutRollDialog::on_saveButton_clicked()
{
    if (cutWidth != 0){
        on_cutButton_clicked();
    }

    int rolls2Cut = cutWidthModel->rowCount(QModelIndex());
    if ((rolls2Cut == 1) || (rollAmount == 0)){
            QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
               QString::fromUtf8("Нечего резать"), QMessageBox::Ok);
        return;
    }

    if (lengthM < PPLib::rollLengthM) {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
           QString::fromUtf8("Рулон не целый. \nДлина рулона %1 м").arg(lengthM),
           QMessageBox::Ok);
        return;
    }

    MainWindow::db->transaction();

    for (int i = 0; i < rolls2Cut; i++){
        int cutWidth = cutWidthModel->sibling(i, 0, QModelIndex()).data().toInt();
        Size cutSize(cutWidth, PPLib::rollLength);

        Stock *newStock = findStock(cutWidth);
        if (newStock != NULL) {
            bool oldStockIncrement = stock->increment("stock_amount", -cutSize.value);
            bool newStockIncrement = newStock->increment("stock_amount", cutSize.value);

            if (!newStockIncrement || !oldStockIncrement){
                MainWindow::db->rollback();
                return;
            }
        } else {
            newStock = new Stock(stock);
            newStock->fields.remove("stock_id");
            newStock->fields["stock_param"] = cutWidth;
            newStock->fields["stock_amount"] = cutSize.value;

            bool oldStockIncrement = stock->increment("stock_amount", -cutSize.value);
            if (!newStock->update() || !oldStockIncrement){
                MainWindow::db->rollback();
                delete newStock;
                return;
            }
            widthModel->setData(QModelIndex(), QVariant((int) newStock), PPLib::addRole);
        }
    }

    MainWindow::db->commit();
    QDialog::accept();
}

void CutRollDialog::on_closeButton_clicked()
{
    QDialog::reject();
}

void CutRollDialog::entity2Form(Stock *stock)
{
    ui->name->setText(stock->fields["stock_name"].toString());
    width = stock->fields["stock_param"].toInt();
    Size size = stock->fields["stock_amount"];
    lengthM = size.getLengthM(width);
    ui->width->setValue(width);
    ui->cutWidth->setValue(width);

    rollAmount = PPLib::calcRollsAmount(stock->fields["stock_param"].toInt(),
            stock->fields["stock_amount"].toInt());

    ui->rollAmount->setValue(rollAmount);
}

Stock *CutRollDialog::findStock(int width)
{
    for (int i = 0; i < widthModel->rowCount(); i++) {
        QModelIndex index = widthModel->index(i, StockModel::PARAMETR);
        if (width == index.data().toInt()) {
            return (Stock *) index.data(PPLib::entityRole).toInt();
        }
    }

    return NULL;
}


void CutRollDialog::on_cutWidth_valueChanged(int value)
{
    if (value > width){
        cutWidth = width;
        ui->cutWidth->setValue(width);
        return;
    }
    cutWidth = value;
}

void CutRollDialog::on_resetButton_clicked()
{
    entity2Form(stock);
    cutWidthModel->clear();
}

void CutRollDialog::on_rolls_doubleClicked(const QModelIndex &index)
{
    Stock *stock = (Stock *) index.data(PPLib::entityRole).toInt();
    ui->cutWidth->setValue(stock->fields["stock_param"].toInt());
}
