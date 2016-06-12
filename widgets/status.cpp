#include "status.h"
#include "ui_status.h"
#include "models/varmodel.h"
#include "dialogs/currencyratedialog.h"
#include <QDebug>
#include "entity/message.h"
#include "models/modelmanager.h"
#include "mainwindow.h"

Status::Status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Status)
{
    ui->setupUi(this);
    ui->rateBox->installEventFilter(this);
    ui->messageBox->installEventFilter(this);
}

Status::~Status()
{
    delete ui;
}

void Status::start()
{
    var = ModelManager::getInstance()->getVarModel();
    Client *client = Client::currentClient;

    ui->name->setText(client->fields["client_name"].toString());
    ui->role->setText(client->getRoleName());

    if (client->currentRole == Client::PRINT ||
        client->currentRole == Client::POSTPRINT) {

        ui->rateBox->hide();

    } else {
        connect(var, SIGNAL(modelReset()), this, SLOT(varReset()));
    }

    timer = new QTimer(this);
    timer->setInterval(var->getVar(Var::VARREFRESH).toInt());
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start();
    refresh();
}

void Status::clear()
{
    ui->name->setText(QString::fromUtf8("Нет подключения"));
    ui->role->setText(QString::fromUtf8("Нет подключения"));
    disconnect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    delete timer;
}

void Status::refresh()
{
    var->refresh();
    timer->setInterval(var->getVar(Var::VARREFRESH).toInt());

    int amount = Message::checkNewMessage(Client::currentClient->getId());
    ui->newMessagesAmount->setText(QString::number(amount));
}

void Status::varReset()
{
    ui->currencyRate->setText(Money(var->getVar(Var::RATE)).toString());
}


bool Status::eventFilter(QObject *o, QEvent *e)
{
    if (MainWindow::db == NULL || !MainWindow::db->isOpen()) {
        return false;
    }

    if (o == ui->rateBox && e->type() == QEvent::MouseButtonDblClick) {
        CurrencyRateDialog dialog;
        dialog.exec();
        return true;

    } else if (o == ui->messageBox && e->type() == QEvent::MouseButtonDblClick) {
        MainWindow::mainWindow->openMessage();
        return true;
    }
    return false;
}
