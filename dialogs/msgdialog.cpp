#include "msgdialog.h"
#include "ui_msgdialog.h"
#include "models/modelmanager.h"
#include <QDateTime>

MsgDialog::MsgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MsgDialog)
{
    ui->setupUi(this);

    ModelManager *models = ModelManager::getInstance();
    ui->clients->setModel(models->getClientModel());
}

MsgDialog::~MsgDialog()
{
    delete ui;
}

void MsgDialog::setModel(QAbstractItemModel *model, int mode, const QModelIndex &index)
{
    this->model = model;
    this->index = index;
    this->mode = mode;

    if (mode == PPLib::addRole) {
        ui->isReady->setDisabled(true);
        ui->isReject->setDisabled(true);
        if (index.isValid()){
            view = (Entity *) model->data(index, PPLib::entityRole).toInt();
            entity2Form(view);
        }

    } else if (mode == Qt::EditRole){
        ui->subj->setAttribute(Qt::WA_TransparentForMouseEvents);
        ui->subj->setFocusPolicy(Qt::NoFocus);
        ui->clients->setAttribute(Qt::WA_TransparentForMouseEvents);
        ui->clients->setFocusPolicy(Qt::NoFocus);

        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);

    }

}

void MsgDialog::on_exitButton_clicked()
{
    QDialog::reject();
}

void MsgDialog::on_saveButton_clicked()
{
    Message *saveMsg = NULL;

    if (mode == PPLib::addRole) {
        saveMsg = new Message();

    } else if (mode == Qt::EditRole) {
        saveMsg = new Message((Message *)view);
    }

    if (!form2Entity(saveMsg) || !saveMsg->update()) {

        return;
    }

    model->setData(index, (int) saveMsg, mode);
    QDialog::accept();
}

void MsgDialog::entity2Form(Entity *msg)
{
    int index = ui->clients->findData(msg->fields["client_from"], PPLib::idRole);
    ui->clients->setCurrentIndex(index);
    ui->subj->setText(msg->fields["message_subj"].toString());
    ui->text->setPlainText(msg->fields["message_log"].toString());
    int status = msg->fields["message_status"].toInt();

    ui->isReady->setChecked(status == Message::PERFORM);
    ui->isReject->setChecked(status == Message::REJECT);

    int clientTo = msg->fields["client_to"].toInt();
    if (mode == Qt::EditRole &&
        status == Message::NEWMSG &&
        clientTo == Client::currentClient->getId()) {

        msg->fields["message_status"] = Message::READ;
        msg->update();
    }
    if (status == Message::PERFORM) {
        ui->saveButton->hide();
    }
}

bool MsgDialog::form2Entity(Entity *msg)
{
    Message *message = (Message *) msg;

    QString text = ui->subj->text();
    if (text.length() == 0) {
        PPLib::fillFormMessage();
        return false;
    }

    msg->fields["client_from"] = Client::currentClient->getId();
    msg->fields["client_to"] = ui->clients->itemData(ui->clients->currentIndex(), PPLib::idRole);
    msg->fields["message_subj"] = text;
    msg->fields["message_log"] = ui->text->toPlainText();
    msg->fields["message_date"] = QDateTime::currentDateTime();
    message->setMesgTo(ui->clients->currentText());
    message->setMesgFrom(Client::currentClient->fields["client_name"]);

    if (ui->isReady->isChecked()) {
        msg->fields["message_status"] = Message::PERFORM;

    } else if (ui->isReject->isChecked()) {
        msg->fields["message_status"] = Message::REJECT;
    }

    return true;
}
