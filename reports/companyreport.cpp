#include "companyreport.h"
#include "ui_companyreport.h"
#include "widgets/entitytable.h"
#include <QDate>
#include <QTimer>

CompanyReport::CompanyReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanyReport), customerModel(NULL), supplyModel(NULL)
{
    ui->setupUi(this);
    for (int i = 0; i < Company::companies.length(); i++){
            ui->type->addItem(Company::companies[i], i);
    }

    setWindowTitle(QString::fromUtf8("Отчет: по компании"));

    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");

    QDate current = QDate::currentDate();
    ui->dateTill->setDate(current);
    ui->dateFrom->setDate(current.addMonths(-1));

    popMessage = new PopUpWindow(this, QString::fromUtf8("создание отчета"));
    connect(ui->company, SIGNAL(companyChanged(Company*)), this, SLOT(companyChanged(Company *)));
}

CompanyReport::~CompanyReport()
{
    delete ui;
}

void CompanyReport::on_startButton_clicked()
{
    popMessage->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTimer::singleShot(100, this, SLOT(execute()));
}

void CompanyReport::execute()
{
    Money total, debt;
    Size size;
    int index = ui->type->currentIndex();

    Company *company = ui->company->getCompany();

    if (company == NULL) {
        return;
    }

    if (index == Company::CUSTOMER) {
        CustomerReportM *customerModel = (CustomerReportM *) model;
        QSqlQuery &query = customerModel->getQuery();

        query.bindValue(":id", company->getId());
        query.bindValue(":from", ui->dateFrom->date());
        query.bindValue(":till", ui->dateTill->date());

        query.exec();

        customerModel->setQuery(query);
        customerModel->getTotal(total, debt, size);
        ui->customerTotal->setText(total.toString());
        ui->customerDebt->setText(debt.toString());
        ui->customerSize->setText(size.toString());

    } else if (index == Company::SUPPLIER) {
        SupplyReportM *supplyModel = (SupplyReportM *) model;
        QSqlQuery &query = supplyModel->getQuery();

        query.bindValue(":id", company->getId());
        query.bindValue(":from", ui->dateFrom->date());
        query.bindValue(":till", ui->dateTill->date());

        query.exec();

        supplyModel->setQuery(query);
        supplyModel->getTotal(total, debt);

        ui->supplyTotal->setText(total.toString());
        ui->supplyDebt->setText(debt.toString());
    }

    popMessage->hide();
    QApplication::restoreOverrideCursor();

}

void CompanyReport::on_type_currentIndexChanged(int index)
{
    ui->company->setup(index, true, false, false, false);
    if (index == Company::CUSTOMER){
        ui->supplyFrame->hide();
        ui->customerFrame->show();

        if (customerModel == NULL) {
            customerModel = new CustomerReportM(this);
        }

        model = customerModel;
        ui->table->setModel(model);

    } else if (index == Company::SUPPLIER) {
        ui->customerFrame->hide();
        ui->supplyFrame->show();

        if (supplyModel == NULL) {
            supplyModel = new SupplyReportM(this);
        }

        model = supplyModel;
        ui->table->setModel(model);
    }
}

void CompanyReport::companyChanged(Company *)
{
    ui->customerTotal->clear();
    ui->customerDebt->clear();
    ui->customerSize->clear();

    ui->supplyTotal->clear();
    ui->supplyDebt->clear();

    model->setQuery(QSqlQuery());
}
