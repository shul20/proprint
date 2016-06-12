#ifndef INVOICE_H
#define INVOICE_H

#include <QWidget>

namespace Ui {
class Invoice;
}

class Invoice : public QWidget
{
    Q_OBJECT
    
public:
    explicit Invoice(QWidget *parent = 0);
    ~Invoice();
    bool isChache();
    const QString getInvoice();
    void setInvoice(const QString &text);
    void setCache(bool set);
    
private slots:
    void on_isCache_toggled(bool checked);

private:
    Ui::Invoice *ui;
};

#endif // INVOICE_H
