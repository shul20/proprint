#ifndef COMPANYREPORT_H
#define COMPANYREPORT_H

#include <QWidget>
#include "widgets/popupwindow.h"
#include "customerreportm.h"
#include "supplyreportm.h"
#include "reportmodel.h"
#include "entity/company.h"

namespace Ui {
class CompanyReport;
}

class CompanyReport : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompanyReport(QWidget *parent = 0);
    ~CompanyReport();
    
private slots:
    void on_startButton_clicked();
    void execute();
    void on_type_currentIndexChanged(int index);
    void companyChanged(Company *);

private:
    Ui::CompanyReport *ui;
    PopUpWindow *popMessage;
    CustomerReportM *customerModel;
    SupplyReportM *supplyModel;
    ReportModel *model;
};

#endif // COMPANYREPORT_H
