#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class LogInDialog;
}

class LogInDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::LogInDialog *ui;
    QSqlDatabase *db;

public:

    explicit LogInDialog(QWidget *parent, QSqlDatabase *db);
    ~LogInDialog();
    
private slots:

    void on_enterButton_clicked();

    void on_cancelButton_clicked();

};

#endif // LOGINDIALOG_H
