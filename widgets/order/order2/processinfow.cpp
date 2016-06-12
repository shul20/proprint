#include "processinfow.h"
#include "ui_processinfow.h"
#include "models/clientmodel.h"
#include "models/modelmanager.h"

ProcessInfoW::ProcessInfoW(QWidget *parent) :
    Order2IWidget(parent),
    ui(new Ui::ProcessInfoW)
{
    ui->setupUi(this);

    setObjectName(QString::fromUtf8("Рабочий процесс"));

    QString color = QString("background: rgba(62,243,204,45)");
    setStyleSheet(QString("QToolButton {%1} QFrame {%1}") .arg(color));

}

ProcessInfoW::~ProcessInfoW()
{
    delete ui;
}

void ProcessInfoW::clear()
{
}

void ProcessInfoW::entity2Form(Entity *order)
{
    if (order->fields["ordr_startp"].isNull()) {
        start = QDateTime::currentDateTime();
        end = order->fields["ordr_end"].toDateTime();
        predict = end;

    } else {
        start = order->fields["ordr_startp"].toDateTime();
        end = order->fields["ordr_endp"].toDateTime();
        predict = order->fields["ordr_predict"].toDateTime();

        ClientModel *model = ModelManager::getInstance()->getClientModel();

        QVariant startId = order->fields["client_start"];
        whoStart = model->getName(startId).toString();

        QVariant endId = order->fields["client_end"];
        whoEnd = model->getName(endId).toString();

        ui->log->setPlainText(order->fields["ordr_info"].toString());
    }
    showData();
}

bool ProcessInfoW::form2Entity(Entity *order)
{

    Client *client = Client::currentClient;
    int clientId = client->getId();
    int status = order->fields["ordr_status"].toInt();
    QDateTime current = QDateTime::currentDateTime();

    if (order->fields["ordr_startp"].isNull()) {
        start = current;
        order->fields["ordr_startp"] = start;

        end = order->fields["ordr_end"].toDateTime();
        order->fields["ordr_endp"] = order->fields["ordr_end"];

        order->fields["client_start"] = clientId;
        order->fields["client_end"] = clientId;

        QString clientName = client->fields["client_name"].toString();
        whoStart = clientName;
        whoEnd = clientName;
    }

    predict = QDateTime(ui->datePredict->date(),ui->timePredict->time());
    order->fields["ordr_predict"] = predict;

    if (status == Order::READY) {
        order->fields["ordr_endp"] = current;
        order->fields["ordr_predict"] = current;
        end = current;
    }

    order->fields["ordr_info"] = ui->log->toPlainText();
    showData();

    return true;
}

void ProcessInfoW::setReadOnly()
{}

void ProcessInfoW::setWidgetReadOnly()
{
    ui->log->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->datePredict->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->timePredict->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void ProcessInfoW::showData()
{
    ui->dateStart->setDateTime(start);
    ui->datePredict->setDate(predict.date());
    ui->timePredict->setTime(predict.time());
    ui->dateEnd->setDateTime(end);
    ui->whoStart->setText(whoStart);
    ui->whoEnd->setText(whoEnd);
}

void ProcessInfoW::on_log_textChanged()
{
    checkText(ui->log->toPlainText());
}
