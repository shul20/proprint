#include "rightswidget.h"
#include "ui_rightswidget.h"
#include "models/modelmanager.h"
#include "widgets/order/order2widget.h"

RightsWidget::RightsWidget(QWidget *parent) :
    Order2IWidget(parent),
    ui(new Ui::RightsWidget)
{
    ui->setupUi(this);
    setObjectName(QString::fromUtf8("Доступ"));

    QString color = QString("background: rgba(255,185,64,85)");
    setStyleSheet(QString("QToolButton {%1} QFrame {%1}") .arg(color));

    managerF = new SortFilterModel(this);
    managerF->compare = &PPLib::checkBits;
    managerF->setFilterRole(ClientModel::roleRole);

    operatorF = new SortFilterModel(this);
    operatorF->compare = &PPLib::checkBits;
    operatorF->setFilterRole(ClientModel::roleRole);

    if (Client::currentRole == Client::PRINT ||
        Client::currentRole == Client::POSTPRINT) {

        ui->managerBox->setDisabled(true);
    }
}

RightsWidget::~RightsWidget()
{
    delete ui;
}

void RightsWidget::clear()
{
    ui->managerBox->setChecked(false);
    ui->operatorBox->setChecked(false);
}

void RightsWidget::entity2Form(Entity *order)
{
    this->order = order;

    ClientModel *model = ModelManager::getInstance()->getClientModel();
    QVariant operatorId = order->fields["client_operate"];
    QVariant managerId = order->fields["client_id"];
    ui->operatorName->setText(model->getName(operatorId).toString());

    if (order->getId() != 0) {
        ui->managerName->setText(model->getName(managerId).toString());
    } else {
        Client *client = Client::currentClient;
        ui->managerName->setText(client->fields["client_name"].toString());
    }
}

bool RightsWidget::form2Entity(Entity *order)
{
    if (ui->managerBox->isChecked()) {
        int index = ui->managers->currentIndex();
        if (index < 0) {
            return false;
        }
        Client *manager = (Client *) ui->managers->itemData(index, PPLib::entityRole).toInt();
        QString name = manager->fields["client_name"].toString();

        order->fields["client_id"] = manager->getId();
        ui->managerName->setText(name);
    }
    if (ui->operatorBox->isChecked()) {
        if (ui->checkForAll->isChecked()){
            order->fields["client_operate"] = QVariant();
            ui->operatorName->setText(QString::fromUtf8("не установлен"));

        } else {
            int index = ui->operators->currentIndex();
            if (index < 0) {
                return false;
            }
            Client *oper = (Client *) ui->operators->itemData(index, PPLib::entityRole).toInt();
            order->fields["client_operate"] = oper->getId();
            ui->operatorName->setText(oper->fields["client_name"].toString());
        }
        return true;
    }

    if ( order->getId() == 0 ) {
        order->fields["client_operate"] = Client::currentClient->getId();
        ui->operatorName->setText(Client::currentClient->fields["client_name"].toString());
    }
    return true;
}

void RightsWidget::setReadOnly()
{
}

void RightsWidget::setWidgetReadOnly()
{
    ui->managerBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->operatorBox->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void RightsWidget::on_managerBox_clicked(bool checked)
{
    if (!checked) {
        ui->managers->setDisabled(true);
        return;
    }
    ModelManager *models = ModelManager::getInstance();
    managerF->setSourceModel(models->getClientModel());

    managerF->setMatchValue(1 << Client::MANAGER);
    ui->managers->setModel(managerF);
    ui->managers->setEnabled(true);
}

void RightsWidget::on_checkForAll_clicked(bool checked)
{
    if (checked) {
        ui->operators->setDisabled(true);
        return;
    }
    operatorF->setSourceModel(ModelManager::getInstance()->getClientModel());

    operatorF->setMatchValue((1 << Client::PRINT) | (1 << Client::POSTPRINT));
    ui->operators->setModel(operatorF);
    ui->operators->setEnabled(true);
}
