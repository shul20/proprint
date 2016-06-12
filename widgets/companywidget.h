#ifndef COMPANYWIDGET_H
#define COMPANYWIDGET_H

#include <QWidget>
#include <QAbstractItemModel>
#include "models/sortfiltermodel.h"
#include "models/contactmodel.h"
#include "entity/company.h"

namespace Ui {
class CompanyWidget;
}

class CompanyWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompanyWidget(QWidget *parent = 0);
    ~CompanyWidget();
    void setup(int companyType, bool isFin, bool isContact, bool isAdd, bool isLabel);

    Company *getCompany();
    void setCompany(int companyId);

    Contact *getContact();
    void setContact(int companyId, int contactId);

    void blockInput(bool check);
    void setReadOnly();
    void updateInfo();
    
private slots:
    void on_company_currentIndexChanged(int index);

    void on_addCompany_clicked();

private:
    Ui::CompanyWidget *ui;
    SortFilterModel *contactF;
    int companyType;

signals:
    void companyChanged(Company *);
};

#endif // COMPANYWIDGET_H
