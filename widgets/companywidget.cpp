#include "companywidget.h"
#include "ui_companywidget.h"
#include "entity/company.h"
#include "models/contactmodel.h"
#include "dialogs/companydialog.h"

#include "models/modelmanager.h"

CompanyWidget::CompanyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanyWidget)
{
    ui->setupUi(this);
    ui->company->setMaxVisibleItems(10);

}

CompanyWidget::~CompanyWidget()
{
    delete ui;
}

Company *CompanyWidget::getCompany()
{
    int index = ui->company->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Company *) ui->company->itemData(index, PPLib::entityRole).toInt();
}

void CompanyWidget::setCompany(int companyId)
{
    int index = ui->company->findData(companyId, PPLib::idRole);
    if (index < 0) {
        Company *company = new Company();
        if (!company->retrive(companyId, false)){
            delete company;
            return;
        }
        ui->company->model()->setData(QModelIndex(), (int)company, PPLib::addRole);
        ui->company->setCurrentIndex(ui->company->count() - 1);
        return;
    }
    ui->company->setCurrentIndex(index);
}

Contact *CompanyWidget::getContact()
{
    int index = ui->contact->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Contact *) ui->contact->itemData(index, PPLib::entityRole).toInt();
}

void CompanyWidget::setContact(int companyId, int contactId)
{
    setCompany(companyId);

    int contactIndex = ui->contact->findData(contactId, PPLib::idRole);
    if (contactIndex < 0) {
        Contact *contact = new Contact();
        if (!contact->retrive(contactId, false)) {
            delete contact;
            return;
        }
        ui->contact->model()->setData(QModelIndex(), (int) contact, PPLib::addRole);
        ui->contact->setCurrentIndex(ui->contact->count() - 1);
        return;

    }
    ui->contact->setCurrentIndex(contactIndex);
}

void CompanyWidget::blockInput(bool check)
{
    ui->company->setDisabled(check);
    ui->addCompany->setDisabled(check);
}

void CompanyWidget::setReadOnly()
{
    ui->company->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->company->setFocusPolicy(Qt::NoFocus);
    ui->contact->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->contact->setFocusPolicy(Qt::NoFocus);
    ui->addCompany->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->addCompany->setFocusPolicy(Qt::NoFocus);
}

void CompanyWidget::updateInfo()
{
    int index = ui->company->currentIndex();
    if (index < 0) {
        return;
    }
    Company *company = (Company *) ui->company->itemData(index, PPLib::entityRole).toInt();

    Money credit = company->fields["company_credit"];
    ui->creditCustomer->setText(credit.toString());

    Money debt = company->fields["company_debt"];
    ui->debtCustomer->setText(debt.toString());
}

void CompanyWidget::setup(int companyType, bool isFin, bool isContact,
                          bool isAdd, bool isLabel)
{
    ui->finFrame->setHidden(!isFin);
    ui->contactFrame->setHidden(!isContact);
    ui->addCompany->setHidden(!isAdd);
    ui->typeLabel->setHidden(!isLabel);

    this->companyType = companyType;

    ModelManager *models = ModelManager::getInstance();

    if (isContact) {
        contactF = new SortFilterModel(this);
        contactF->setFilterRole(ContactModel::companyIdRole);
        contactF->setSourceModel(models->getContactModel());
        ui->contact->setModel(contactF);
    }

    SortFilterModel *companyF = new SortFilterModel(this);
    companyF->setFilterRole(CompanyModel::typeRole);
    companyF->setMatchValue(companyType);
    companyF->setSourceModel(models->getCompanyModel());
    ui->company->setModel(companyF);

    if (companyType == Company::CUSTOMER) {
        ui->typeLabel->setText(QString::fromUtf8("Заказчики"));

    } else if  (companyType == Company::SUPPLIER) {
        ui->typeLabel->setText(QString::fromUtf8("Поставщики"));

    }  else if  (companyType == -1) {
        ui->typeLabel->setText(QString::fromUtf8("Компании"));
    }
}

void CompanyWidget::on_company_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }
    Company *company = (Company *) ui->company->itemData(index, PPLib::entityRole).toInt();

    if (!ui->contactFrame->isHidden()) {
        contactF->setMatchValue(company->getId());
    }

    if (!ui->finFrame->isHidden()){
        Money credit = company->fields["company_credit"];
        ui->creditCustomer->setText(credit.toString());

        Money debt = company->fields["company_debt"];
        ui->debtCustomer->setText(debt.toString());
    }
    emit companyChanged(company);
}


void CompanyWidget::on_addCompany_clicked()
{
    int row = ui->company->currentIndex();
    QAbstractItemModel *model = ui->company->model();
    QModelIndex index = model->index(row, 0);

    CompanyDialog *dialog = new CompanyDialog(this);
    dialog->set(ui->company->model(), PPLib::addRole, index);
    dialog->disableCompanyType(true);
    dialog->clear();

    if (dialog->exec() == QDialog::Accepted) {
        ui->company->setCurrentIndex(ui->company->count() - 1);
        ui->contact->setCurrentIndex(0);
    }

    delete dialog;
}
