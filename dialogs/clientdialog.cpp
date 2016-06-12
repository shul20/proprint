#include "clientdialog.h"
#include "ui_clientdialog.h"
#include <QMessageBox>
#include <QDate>
#include "proprintlib.h"

ClientDialog::ClientDialog(QWidget *parent) :QDialog(parent),
    ui(new Ui::ClientDialog) {

    ui->setupUi(this);

    for (int i = 0; i < Client::roles.length(); i++){
        ui->role->addItem(Client::roles[i], i);
    }
}

ClientDialog::~ClientDialog(){
    delete ui;
}

void ClientDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
{
    this->model = model;
    this->index = index;
    this->mode = mode;

    if (mode == PPLib::addRole) {
        if (index.isValid()){
            view = (Entity *) model->data(index, PPLib::entityRole).toInt();
            entity2Form(view);
        } else {
            clear();
        }
        ui->saveClientButton->setText(QString::fromUtf8("Добавить"));
        setWindowTitle(QString::fromUtf8("Добавить клиента"));

    } else if (mode == Qt::EditRole) {
        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);
        ui->saveClientButton->setText(QString::fromUtf8("Изменить"));
        setWindowTitle(QString::fromUtf8("Редактирование клиента"));
    }
}

void ClientDialog::clear()
{
    ui->name->clear();
    ui->role->setCurrentIndex(0);
    ui->password->clear();
    ui->passwordRepeat->clear();
}


void ClientDialog::on_saveClientButton_clicked()
{
    Client *saveClient = NULL;

    if (mode == PPLib::addRole) {
        saveClient = new Client();

    } else if (mode == Qt::EditRole) {
        saveClient = new Client((Client *)view);
    }

    if (!form2Entity(saveClient) ||
        !saveClient->update()) {

        delete saveClient;
        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), Client::lastError);
        return;
    }

    model->setData(index, (int) saveClient, mode);
    QDialog::accept();
}

void ClientDialog::entity2Form(Entity *client)
{
    ui->name->setText(client->fields["client_name"].toString());
    ui->role->setCurrentIndex(client->fields["client_role"].toInt());
}


bool ClientDialog::form2Entity(Entity *client)
{
    const QString name = ui->name->text();
    const QString password = ui->password->text();
    const QString passwordRepeat = ui->passwordRepeat->text();

    if (name.length() == 0 || password.length() == 0) {
        PPLib::fillFormMessage();
        return false;
    }

    if (password != passwordRepeat) {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
           QString::fromUtf8("Пароли разные"), QMessageBox::Ok);
        return false;
    }

    client->fields["client_name"] = name;
    client->fields["client_role"] = ui->role->currentIndex();
    client->fields["client_password"] =  password;
    client->fields["client_update"] = QDate::currentDate();
    return true;
}



void ClientDialog::on_exitClientButton_clicked()
{
    QDialog::reject();
}
