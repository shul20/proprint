#include "modelmanager.h"

ModelManager *ModelManager::instance = NULL;

ModelManager::ModelManager(QObject *parent) :
    QObject(parent)
{
    varModel = NULL;
    companyModel = NULL;
    contactModel = NULL;
    equipModel = NULL;
    modeModel = NULL;
    passModel = NULL;
    profileModel = NULL;
    priceTypeModel = NULL;
    productModel = NULL;
    clientModel = NULL;
    tasksPrintModel = NULL;
    tasksPostModel = NULL;
    accountModel = NULL;
    payModel = NULL;
    stockModel = NULL;
}

ModelManager *ModelManager::getInstance()
{
    if (instance == NULL) {
        instance = new ModelManager();
    }
    return instance;
}

VarModel *ModelManager::getVarModel()
{
    if (varModel == NULL) {
        varModel = new VarModel();
        varModel->refresh();
    }
    return varModel;
}

CompanyModel *ModelManager::getCompanyModel()
{
    if (companyModel == NULL) {
        companyModel = new CompanyModel(this);
        QSqlQuery &query = companyModel->getRefreshQuery();
        query.prepare(Company::allQuery);
        companyModel->refresh();
    }

    return companyModel;
}

ContactModel *ModelManager::getContactModel()
{
    if (contactModel == NULL) {
        contactModel = new ContactModel(this);
        QSqlQuery &query = contactModel->getRefreshQuery();
        query.prepare(Contact::allQuery);
        contactModel->refresh();
    }

    return contactModel;
}

EquipmentModel *ModelManager::getEquipModel()
{
    if (equipModel == NULL) {
        equipModel = new EquipmentModel(this);
        QSqlQuery &query = equipModel->getRefreshQuery();
        query.prepare(Equipment::allQuery);
        equipModel->refresh();
    }

    return equipModel;
}

PmodeModel *ModelManager::getModeModel()
{
    if (modeModel == NULL) {
        modeModel = new PmodeModel(this);
        QSqlQuery &query = modeModel->getRefreshQuery();
        query.prepare(Pmode::allQuery);
        modeModel->refresh();
    }

    return modeModel;
}

PassModel *ModelManager::getPassModel()
{
    if (passModel == NULL) {
        passModel = new PassModel(this);
        QSqlQuery &query = passModel->getRefreshQuery();
        query.prepare(Pass::allQuery);
        passModel->refresh();
    }

    return passModel;
}

ProfileModel *ModelManager::getProfileModel()
{
    if (profileModel == NULL) {
        profileModel = new ProfileModel(this);
        QSqlQuery &query = profileModel->getRefreshQuery();
        query.prepare(Profile::allQuery);
        profileModel->refresh();
    }

    return profileModel;
}

PriceTypeModel *ModelManager::getPriceTypeModel()
{
    if (priceTypeModel == NULL) {
        priceTypeModel = new PriceTypeModel(this);
        QSqlQuery &query = priceTypeModel->getRefreshQuery();
        query.prepare(PriceType::allQuery);
        priceTypeModel->refresh();
    }

    return priceTypeModel;
}

ProductModel *ModelManager::getProductModel()
{
    if (productModel == NULL) {
        productModel = new ProductModel(this);
        QSqlQuery &query = productModel->getRefreshQuery();
        query.prepare(Product::allQuery);
        productModel->refresh();
    }

    return productModel;
}

ClientModel *ModelManager::getClientModel()
{
    if (clientModel == NULL) {
        clientModel = new ClientModel(this);
        QSqlQuery &query = clientModel->getRefreshQuery();
        query.prepare(Client::allQuery);
        clientModel->refresh();
    }

    return clientModel;
}

TasksPrintM *ModelManager::getTasksPrintModel()
{
    if (tasksPrintModel == NULL) {
        tasksPrintModel = new TasksPrintM(this);
    }

    return tasksPrintModel;
}

TaskPostM *ModelManager::getTasksPostModel()
{
    if (tasksPostModel == NULL) {
        tasksPostModel = new TaskPostM(this);
    }

    return tasksPostModel;
}

AccountModel *ModelManager::getAccountModel()
{
    if (accountModel == NULL) {
        accountModel = new AccountModel(this);
        accountModel->refresh();
    }

    return accountModel;
}

PayModel *ModelManager::getPayModel()
{
    if (payModel == NULL) {
        payModel = new PayModel(this);
        payModel->refresh();
    }

    return payModel;

}

StockModel *ModelManager::getStockModel()
{
    if (stockModel == NULL) {
        stockModel = new StockModel(this);
        stockModel->refresh();
    }

    return stockModel;
}
