#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include "entity/entity.h"
#include "models/order/workmodel.h"
#include "models/order/taskmodel.h"
#include "widgets/work/task/taskwidget.h"
#include "widgets/companywidget.h"
#include "widgets/stock/pricetypewidget.h"
#include "models/sortfiltermodel.h"

class WorkTable;

class WorkWidget : public QWidget
{
    Q_OBJECT
protected:
    WorkModel *workModel;
    TaskModel *taskModel;
    SortFilterModel *taskF;
    WorkTable *table;
    PriceTypeWidget *priceTypeW;
    CompanyWidget *companyW;
    SizeWidget *sizeW;
    Size totalSize;
    Money total, totalPrint, workTotal, workTotalPrint;
    TaskWidget *mainWidget;

    QList<TaskWidget *> tasks;
    QMap<int, TaskWidget *> taskMap;
    int mainProcess, idCounter;

    void calcWorkTotal();
    void resetId();

public:
    explicit WorkWidget(QWidget *parent = 0);

    virtual bool update(int orderId) = 0;
    virtual ProductWidget *getProductW() = 0;
    virtual Entity *getEquip() = 0;
    virtual void clear() = 0;
    virtual void setOrder(int orderId, bool isCopy = false) = 0;

    void setWidgets(PriceTypeWidget *priceType, CompanyWidget *company);
    Size &getTotalSize();
    void setTotalSize(Size value);

    void setTotal(Money total, Money print);
    Money &getTotal();
    Money &getTotalPrint();

    bool addTasks2Work(Entity *work);
    void tasks2Forms(int workId);
    bool forms2Tasks(Entity *work);
    bool delTasksFromWork(Entity *work);
    void clearTasks();

protected slots:
    void registerTask(bool check);
    virtual void totalChanged() = 0;

signals:
    void worksChaged();
    void blockInput(bool);
    
};

#endif // WORKWIDGET_H
