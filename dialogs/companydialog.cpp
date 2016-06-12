#include "companydialog.h"
#include "ui_companydialog.h"
#include "proprintlib.h"
#include "mainwindow.h"
#include <QListWidget>
#include <QCompleter>
#include <QSqlError>
#include <QSqlQueryModel>

CompanyDialog::CompanyDialog(QWidget *parent) :QDialog(parent),
    ui(new Ui::CompanyDialog), contacts(NULL)
{
    ui->setupUi(this);

    for (int i = 0; i < Company::companies.length(); i++){
            ui->type->addItem(Company::companies[i], i);
    }
    installEventFilter(this);
}

CompanyDialog::~CompanyDialog()
{
    delete ui;
}

void CompanyDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
{
    this->model = model;
    this->index = index;
    this->mode = mode;

    if (contacts != NULL){
        ui->contactsBox->layout()->removeWidget(contacts);
        delete contacts;
    }
    contacts = new ContactView(this);
    ui->contactsBox->layout()->addWidget(contacts);

    if (mode == PPLib::addRole) {
        if (index.isValid()){
            view = (Entity *) model->data(index, PPLib::entityRole).toInt();
            entity2Form(view);
        } else {
            clear();
        }
        ui->saveCompanyButton->setText(QString::fromUtf8("Добавить"));
        setWindowTitle(QString::fromUtf8("Добавить компанию"));
        contacts->setCompany(0);

    } else if (mode == Qt::EditRole) {
        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);
        ui->saveCompanyButton->setText(QString::fromUtf8("Изменить"));
        setWindowTitle(QString::fromUtf8("Редактирование компании"));
        contacts->setCompany(view->getId());
    }
}

void CompanyDialog::disableCompanyType(bool check)
{
    if (check){
        ui->type->setAttribute(Qt::WA_TransparentForMouseEvents);
        ui->type->setFocusPolicy(Qt::NoFocus);
    }
}

void CompanyDialog::clear()
{
    //ui->name->clear();
    ui->address->clear();
    ui->tel->clear();
    ui->account->clear();
    ui->credit->clear();
    ui->debt->clear();
}

void CompanyDialog::on_saveCompanyButton_clicked()
{
    MainWindow::db->transaction();
    Company *saveCompany = NULL;

    if (mode == PPLib::addRole) {
        saveCompany = new Company();
        saveCompany->fields["company_debt"] = 0;

    } else if (mode == Qt::EditRole) {
        saveCompany = new Company((Company *)view);
    }

    if (!form2Entity(saveCompany) ||
        !saveCompany->update() ||
        !contacts->update(saveCompany->getId())) {

        qDebug() << MainWindow::db->lastError().driverText();
        MainWindow::db->rollback();
        delete saveCompany;
        return;
    }

    MainWindow::db->commit();
    model->setData(index, (int) saveCompany, mode);
    QDialog::accept();
}

void CompanyDialog::entity2Form(Entity *company)
{
    ui->name->setText(company->fields["company_name"].toString());
    ui->address->setText(company->fields["company_address"].toString());
    ui->tel->setText(company->fields["company_tel"].toString());
    ui->account->setText(company->fields["company_account"].toString());
    ui->type->setCurrentIndex(company->fields["company_type"].toInt());

    Money credit = company->fields["company_credit"];
    ui->credit->setMoney(credit);

    Money debt(-company->fields["company_debt"].toInt());
    ui->debt->setMoney(debt);
}

bool CompanyDialog::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::Move || e->type() == QEvent::Resize) {
        ui->name->drawList();
    }
    return false;
}

bool CompanyDialog::form2Entity(Entity *company)
{
    QString name = ui->name->text();
    QString address = ui->address->text();
    QString tel = ui->tel->text();
    QString account = ui->account->text();

    if (name.length() == 0 || address.length() == 0 || tel.length() == 0 || account.length() == 0) {
        PPLib::fillFormMessage();
        return false;
    }

    company->fields["company_name"] = name;
    company->fields["company_type"] = ui->type->currentIndex();
    company->fields["company_address"] = address;
    company->fields["company_tel"] = tel;
    company->fields["company_account"] = account;
    company->fields["company_credit"] = ui->credit->getMoney().cents;

    return true;
}

void CompanyDialog::on_exitCompanyButton_clicked()
{
    contacts->exitNoUpdate();
    QDialog::reject();
}
