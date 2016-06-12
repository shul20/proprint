#ifndef DEBTREPORT_H
#define DEBTREPORT_H

#include <QWidget>
#include "widgets/popupwindow.h"
#include "debtreportm.h"

namespace Ui {
class DebtReport;
}

class DebtReport : public QWidget
{
    Q_OBJECT
    
public:
    explicit DebtReport(QWidget *parent = 0);
    ~DebtReport();
    
private slots:
    void on_startButton_clicked();
    void execute();
    void on_type_currentIndexChanged(int index);

private:
    Ui::DebtReport *ui;
    PopUpWindow *popMessage;
    DebtReportM *model;
};

#endif // DEBTREPORT_H
