#ifndef WOFFWIDGET_H
#define WOFFWIDGET_H

#include <QWidget>
#include "entity/stock/stock.h"
#include "entity/order/work.h"
#include "entity/stock/woff.h"
#include "entity/order/task.h"

namespace Ui {
class WoffWidget;
}

class WoffWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit WoffWidget(QWidget *parent = 0);
    ~WoffWidget();
    void setup(Stock *stock);
    bool woffPrepare(Entity *work, Entity *stock, QString &message);
    Woff *createWoff(Entity *work, Entity *stock, QString &message);

    bool update(Woff *woff);
    void woffShow(Entity *entity);
    void woffShow(int);
    
private:
    Ui::WoffWidget *ui;
    Entity *work, *stock;
};

#endif // WOFFWIDGET_H
