#ifndef INVOICEDIALOG_H
#define INVOICEDIALOG_H

#include <QDialog>
#include "entity/entity.h"

namespace Ui {
class InvoiceDialog;
}

class InvoiceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InvoiceDialog(QWidget *parent = 0);
    ~InvoiceDialog();
    void setOrder(Entity * order, Entity *view);
    
private slots:
    void on_exitButton_clicked();

    void on_printButton_clicked();

private:
    Ui::InvoiceDialog *ui;
};

#endif // INVOICEDIALOG_H
