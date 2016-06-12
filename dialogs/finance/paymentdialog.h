#ifndef PAYMENTDIALOG_H
#define PAYMENTDIALOG_H

#include <QDialog>
#include "models/finance/paymentmodel.h"
#include "entity/finance/payment.h"
#include "models/sortfiltermodel.h"

namespace Ui {
class PaymentDialog;
}

class PaymentDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PaymentDialog(QWidget *parent = 0);
    ~PaymentDialog();
    void setModel(QAbstractItemModel *model, int mode, Entity *view = NULL);
    
    enum {ORDER = 0, MATERIAL = 1, OTHER = 2, DEPOSIT = 3} modesEnum;

private slots:
    void on_saveButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::PaymentDialog *ui;
    PaymentModel *paymentModel;
    SortFilterModel *payModelF;
    Entity *view;
    Payment *payment;
    int mode;

    void entity2Form(Entity *payment);
    bool form2Entity(Payment *payment);
    void setupTable(int sourceId, int type);
    void setupInfo(QString &info);
    void retriveContact();

};

#endif // PAYMENTDIALOG_H
