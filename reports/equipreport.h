#ifndef EQUIPREPORT_H
#define EQUIPREPORT_H

#include <QWidget>
#include <QDateTime>
#include "reports/equipreportm.h"
#include "widgets/popupwindow.h"

namespace Ui {
class EquipReport;
}

class EquipReport : public QWidget
{
    Q_OBJECT
    
public:
    explicit EquipReport(QWidget *parent = 0);
    ~EquipReport();
    
private slots:
    void on_startButton_clicked();
    void execute();

private:
    Ui::EquipReport *ui;
    EquipReportM *model;
    PopUpWindow *popMessage;
};

#endif // EQUIPREPORT_H
