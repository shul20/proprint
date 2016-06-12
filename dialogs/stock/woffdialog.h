#ifndef WOFFDIALOG_H
#define WOFFDIALOG_H

#include <QDialog>
#include "entity/entity.h"
#include "models/stock/stockmodel.h"
#include "entity/finance/payment.h"
#include "entity/stock/woff.h"

namespace Ui {
class WoffDialog;
}

class WoffDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WoffDialog(QWidget *parent = 0);
    ~WoffDialog();
    void setModel(QAbstractItemModel *model, Entity *view);
    
private slots:
    void on_saveButton_clicked();

    void on_exitButton_clicked();

    void on_rollSize_textEdited(const QString &arg1);

    void on_rollAmount_valueChanged(int arg1);

private:
    Ui::WoffDialog *ui;
    StockModel *payModel;
    Entity *view;
    QStringList labels;

    void entity2Form(Entity *stock);
    bool form2Entity(Entity *woff, QString &message);
    bool updatePayment(Entity *woff, QString &message);

    static const enum {MM = 0, M2 = 1, L = 2, ROLL = 3, LIST = 4, E = 5} labelEnum;
};

#endif // WOFFDIALOG_H
