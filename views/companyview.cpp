#include "companyview.h"
#include "ui_companyview.h"
#include "models/modelmanager.h"

CompanyView::CompanyView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanyView), findF(NULL)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Компании"));

    ModelManager *models = ModelManager::getInstance();
    companyF = new SortFilterModel(this);
    companyF->setSourceModel(models->getCompanyModel());
    companyF->setFilterRole(CompanyModel::typeRole);

    for (int i = 0; i < Company::companies.length(); i++){
            ui->filter->addItem(Company::companies[i], i);
    }

    //QSortFilterProxyModel *sortModel = new QSortFilterProxyModel(this);
    //sortModel->setSourceModel(models->getCompanyModel());

    ui->table->setModel(companyF);

    connect(ui->table->actionAdd, SIGNAL(triggered()), this, SLOT(addCompany()));
    connect(ui->table->actionEdit, SIGNAL(triggered()), this, SLOT(editCompany()));
    connect(ui->table, SIGNAL(doubleClicked(QModelIndex)),
        this, SLOT(editCompany(QModelIndex)));
}

CompanyView::~CompanyView()
{
    delete ui;
}

void CompanyView::addCompany()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    CompanyDialog *dialog = new CompanyDialog(this);
    if (!select->hasSelection()){
        dialog->set(ui->table->model(), PPLib::addRole, QModelIndex());
    } else {
        dialog->set(ui->table->model(), PPLib::addRole, select->currentIndex());
    }

    dialog->exec();
    delete dialog;
}

void CompanyView::editCompany()
{
    QItemSelectionModel *select = ui->table->selectionModel();

    if (!select->hasSelection()){
        return;
    }

    editCompany(select->currentIndex());
}

void CompanyView::editCompany(QModelIndex index)
{
    CompanyDialog *dialog = new CompanyDialog(this);
    dialog->set(ui->table->model(), Qt::EditRole, index);
    dialog->exec();
    delete dialog;
}



void CompanyView::on_filter_currentIndexChanged(int index)
{
    companyF->setMatchValue(index);
}

void CompanyView::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1.length() < 3) {
        if (ui->table->model() != companyF) {
            ui->table->setModel(companyF);
        }
        return;
    }

    if (findF == NULL) {
        findF = new SortFilterModel(this);
        findF->setSourceModel(companyF);
        findF->setFilterRole(Qt::EditRole);
        findF->compare = &SortFilterModel::contain;
    }
    if (ui->table->model() != findF) {
        ui->table->setModel(findF);
    }

    findF->setMatchValue(arg1);
}
