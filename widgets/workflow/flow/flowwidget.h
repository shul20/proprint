#ifndef FLOWWIDGET_H
#define FLOWWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QGroupBox>
#include "entity/entity.h"
#include "widgets/stock/woffwidget.h"
#include "models/sortfiltermodel.h"
#include "models/order/workmodel.h"

class FlowWidget : public QWidget
{
    Q_OBJECT
protected:
    int status;
    Entity *task, *order, *work;
    SortFilterModel *taskF;
    WorkModel *workModel;
    QToolButton *startButton, *pauseButton, *readyButton;
    QGroupBox *groupBox;
    bool isReady;

    void disableFocus();
    void changeWorkStatus();
    bool checkWeight();
    bool checkEquipment();
    void setStatus(int status);

public:
    explicit FlowWidget(QWidget *parent = 0);
    virtual void entity2Form(Entity *entity) = 0;

    void set(Entity *order, Entity *work, SortFilterModel *taskF, WorkModel *workModel);

    
protected slots:
    void startClicked();
    void pauseClicked();
    virtual void readyClicked();

};

#endif // FLOWWIDGET_H
