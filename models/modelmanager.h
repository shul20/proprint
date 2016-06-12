#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include "models/companymodel.h"
#include "models/contactmodel.h"
#include "models/equip/equipmentmodel.h"
#include "models/equip/pmodemodel.h"
#include "models/equip/passmodel.h"
#include "models/equip/profilemodel.h"
#include "models/stock/pricetypemodel.h"
#include "models/productmodel.h"
#include "models/clientmodel.h"
#include "models/order/tasksprintm.h"
#include "models/order/taskpostm.h"
#include "models/varmodel.h"
#include "models/finance/accountmodel.h"
#include "models/finance/paymodel.h"
#include "models/stock/stockmodel.h"

class ModelManager : public QObject
{
    Q_OBJECT
private:
    static ModelManager *instance;
    explicit ModelManager(QObject *parent = 0);

    VarModel *varModel;
    CompanyModel *companyModel;
    ContactModel *contactModel;
    EquipmentModel *equipModel;
    PmodeModel *modeModel;
    PassModel *passModel;
    ProfileModel *profileModel;
    PriceTypeModel *priceTypeModel;
    ProductModel *productModel;
    ClientModel *clientModel;
    TasksPrintM *tasksPrintModel;
    TaskPostM *tasksPostModel;
    AccountModel *accountModel;
    PayModel *payModel;
    StockModel *stockModel;

public:
    static ModelManager *getInstance();

    VarModel *getVarModel();
    CompanyModel *getCompanyModel();
    ContactModel *getContactModel();
    EquipmentModel *getEquipModel();
    PmodeModel *getModeModel();
    PassModel *getPassModel();
    ProfileModel *getProfileModel();
    PriceTypeModel *getPriceTypeModel();
    ProductModel *getProductModel();
    ClientModel *getClientModel();
    TasksPrintM *getTasksPrintModel();
    TaskPostM *getTasksPostModel();
    AccountModel *getAccountModel();
    PayModel *getPayModel();
    StockModel *getStockModel();

signals:
    
public slots:
    
};

#endif // MODELMANAGER_H
