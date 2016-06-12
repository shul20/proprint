#ifndef CURRENCYRATEDIALOG_H
#define CURRENCYRATEDIALOG_H

#include <QDialog>

namespace Ui {
class CurrencyRateDialog;
}

class CurrencyRateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CurrencyRateDialog(QWidget *parent = 0);
    ~CurrencyRateDialog();
    
private slots:
    void on_saveButton_clicked();

    void on_exitButton_clicked();

    void on_rate_textChanged(const QString &arg1);

private:
    Ui::CurrencyRateDialog *ui;
};

#endif // CURRENCYRATEDIALOG_H
