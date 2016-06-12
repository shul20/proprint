#ifndef STATUS_H
#define STATUS_H

#include <QWidget>
#include <QTimer>

#include "utility/money.h"
#include "entity/client.h"
#include "models/varmodel.h"

namespace Ui {
class Status;
}

class Status : public QWidget
{
    Q_OBJECT
    
public:
    explicit Status(QWidget *parent = 0);
    ~Status();

    void start();
    void clear();
    bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::Status *ui;
    QTimer *timer;
    VarModel *var;

private slots:
    void refresh();
    void varReset();
};

#endif // STATUS_H
