#include "logindialog.h"
#include "ui_logindialog.h"
#include "proprintlib.h"
#include "entity/client.h"

#include <QFile>
#include <QDebug>
#include <QSqlError>

LogInDialog::LogInDialog(QWidget *parent, QSqlDatabase *db) :
    QDialog(parent), ui(new Ui::LogInDialog), db(db)
{
    ui->setupUi(this);
}

LogInDialog::~LogInDialog()
{
    delete ui;
}

void LogInDialog::on_enterButton_clicked(){
    QString user = QString::fromLocal8Bit(ui->user->text().toUtf8());
    QString password = ui->password->text();
    QFile file("config.txt");

    if ((user.length() == 0) || (password.length() == 0)) {
        PPLib::fillFormMessage();
        return;
    }

    if (file.open(QFile::ReadOnly)){
        QStringList config;
        QMap<QString, QString> data;

        config = QString(file.readAll()).split("\r\n");
        foreach (QString item, config) {
            QStringList valuesList = item.simplified().split(" ");
            if (valuesList.size() == 2) {
                data[valuesList[0]] = valuesList[1];
            }

        }
        file.close();

        db->setHostName(data["host"]);
        db->setDatabaseName(data["data"]);

        db->setUserName(user);

        db->setPassword(password);

        db->open();
        if (!db->isOpen()) {
            QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
                   db->lastError().databaseText(), QMessageBox::Ok);

            return;
        }

        Client *client = new Client();
        client->retrive("client_name", ui->user->text());
        Client::currentClient = client;
        Client::currentRole = client->fields["client_role"].toInt();

        QDialog::accept();

    } else {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
           QString::fromUtf8("Не найден файл конфигурации"), QMessageBox::Ok);
    }

}

void LogInDialog::on_cancelButton_clicked()
{
    QDialog::reject();
}
