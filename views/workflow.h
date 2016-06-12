#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <QWidget>
#include <QTimer>
#include "models/companymodel.h"
#include "models/order/tasksprintm.h"
#include "models/order/taskpostm.h"

namespace Ui {
class WorkFlow;
}

class WorkFlow : public QWidget
{
    Q_OBJECT
    
public:
    explicit WorkFlow(QWidget *parent = 0);
    ~WorkFlow();

private:
    Ui::WorkFlow *ui;
    QSqlQuery taskQuery;
    TasksPrintM *tasksPrintM;
    TaskPostM *tasksPostM;
    QTimer *timer;

private slots:
    void refreshTasks();

    void on_viewStartButton_clicked();
    void on_viewAllButton_clicked();
    void on_viewPrintButton_clicked();
    void on_viewPostPrintButton_clicked();
    void on_readyButton_clicked();
};

#endif // WORKFLOW_H
