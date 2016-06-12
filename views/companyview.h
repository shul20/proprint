#ifndef COMPANYVIEW_H
#define COMPANYVIEW_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include "models/sortfiltermodel.h"

namespace Ui {
class CompanyView;
}

class CompanyView : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompanyView(QWidget *parent = 0);
    ~CompanyView();
    
private:
    Ui::CompanyView *ui;
    SortFilterModel *companyF, *findF;


private slots:
    void addCompany();
    void editCompany();
    void editCompany(QModelIndex index);

    void on_filter_currentIndexChanged(int index);
    void on_lineEdit_textChanged(const QString &arg1);
};

#endif // COMPANYVIEW_H
