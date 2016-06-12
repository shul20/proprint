#include "paymentdialog.h"
#include "ui_paymentdialog.h"
#include "entity/client.h"
#include <QDateTime>
#include <QSqlError>
#include "models/varmodel.h"
#include "models/modelmanager.h"
#include "mainwindow.h"
#include "widgets/entitytable.h"
#include "entity/order/order.h"
#include "entity/company.h"
#include "entity/stock/material.h"

PaymentDialog::PaymentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaymentDialog), paymentModel(NULL)
{
    ui->setupUi(this);
    PayModel *payModel = ModelManager::getInstance()->getPayModel();
    payModelF = new SortFilterModel(this);
    payModelF->setSourceModel(payModel);
    ui->pays->setModel(payModelF);
}

PaymentDialog::~PaymentDialog()
{
    delete ui;
}

void PaymentDialog::setModel(QAbstractItemModel *model, int mode, Entity *view)
{
    paymentModel = (PaymentModel *) model;
    this->mode = mode;
    this->view = view;
    payment = new Payment();
    int id;

    if (mode == OTHER) {
        payModelF->setFilterRole(PayModel::otherRole);
        payModelF->setMatchValue(true);

        ui->table->hide();
        adjustSize();
        return;

    } else if (mode == ORDER) {
        id = view->fields["ordr_id"].toInt();

        payModelF->setFilterRole(PayModel::orderRole);
        payModelF->setMatchValue(true);

        QString info = QString::fromUtf8("Заказ №%1  ").arg(id);
        setupInfo(info);
        ui->total->setMoney(Money(view->fields["ordr_debt"]));
        setupTable(id, Pay::ORDER);

    } else if (mode == MATERIAL) {
        id = view->fields["material_id"].toInt();
        Material *material = (Material *) view;

        payModelF->setFilterRole(PayModel::materialRole);
        payModelF->setMatchValue(true);

        QString info = QString::fromUtf8("%1 %2 %3")
                        .arg(material->getProductName())
                        .arg(material->getStockName())
                        .arg(material->getParametr());

        setupInfo(info);
        ui->total->setMoney(Money(view->fields["material_debt"]));

        QString invoice = view->fields["material_invoice"].toString();
        ui->invoice->setInvoice(invoice);
        setupTable(id, Pay::MATERIAL);

    }  else if (mode == DEPOSIT) {
        id = view->fields["payment_field_id"].toInt();
        Payment *depositPayment = (Payment *) view;
        int type = depositPayment->getPayType();
        int paymentId = view->fields["payment_id"].toInt();

        if (type == Pay::ORDER ) {
            payModelF->setFilterRole(PayModel::debtOrderRole);
            setupTable(paymentId, Pay::ORDER);

        } else if (type == Pay::MATERIAL ) {
            payModelF->setFilterRole(PayModel::debtMaterialRole);
            setupTable(paymentId, Pay::MATERIAL);

        } else if (type == Pay::OTHER ) {
            payModelF->setFilterRole(PayModel::debtOtherRole);
            setupTable(paymentId, Pay::OTHER);
        }

        payModelF->setMatchValue(true);

        QString info = QString::fromUtf8("Относится к платежу %1.")
                .arg(paymentId);

        setupInfo(info);
        int cents = abs(view->fields["payment_debt"].toInt());
        ui->total->setMoney(Money(cents));

        QString invoice = view->fields["payment_invoice"].toString();
        ui->invoice->setInvoice(invoice);
    }

     payment->fields["payment_field_id"] = id;

}

void PaymentDialog::on_saveButton_clicked()
{
    MainWindow::db->transaction();
    payment->fields.remove("payment_id");

    if (!form2Entity(payment) ||
        !payment->update()) {

        MainWindow::db->rollback();
        QMessageBox::critical(this, QString::fromUtf8("Ошибка"), Payment::lastError);
        return;
    }

    MainWindow::db->commit();

    if (mode == ORDER){
        retriveContact();

    } else if (mode == MATERIAL){
        Material *material = (Material *)view;
        material->retrive(material->getId(), false);
        retriveContact();

    } else if (mode == DEPOSIT){
        Payment *depositPayment = (Payment *)view;
        int type = depositPayment->getPayType();
        depositPayment->retrive(depositPayment->getId(), false);
        paymentModel->setData(QModelIndex(), (int) payment, PPLib::addRole);

        if (type != Pay::OTHER){
            retriveContact();
        }

    } else if (mode == OTHER){
        paymentModel->setData(QModelIndex(), (int) payment, PPLib::addRole);
    }

    ModelManager::getInstance()->getAccountModel()->refresh();
    close();
}

void PaymentDialog::on_exitButton_clicked()
{
    close();
}

void PaymentDialog::entity2Form(Entity *payment)
{}

bool PaymentDialog::form2Entity(Payment *payment)
{
    QString info = ui->info->toPlainText();
    Money total = ui->total->getMoney();
    QString invoice = ui->invoice->getInvoice();

    if (info.length() == 0 || total == 0 || invoice.length() == 0 ) {
        Payment::lastError = QString::fromUtf8("Заполните поля.");
        return false;
    }
    Pay *pay = (Pay *)ui->pays->itemData(ui->pays->currentIndex(), PPLib::entityRole).toInt();
    payment->fields["pay_id"] = pay->getId();
    payment->fields["client_id"] = Client::currentClient->getId();
    payment->fields["payment_total"] = total.cents;

    payment->fields["payment_invoice"] = invoice;
    payment->fields["payment_iscache"] = ui->invoice->isChache();
    payment->fields["payment_info"] = info;
    payment->fields["payment_date"] = QDateTime::currentDateTime();

    int debtType = pay->fields["pay_debt"].toInt();
    payment->setClient(Client::currentClient->fields["client_name"]);
    payment->setPayType(pay->fields["pay_type"].toInt());
    payment->setPayDebt(debtType);
    payment->setPayName(pay->fields["pay_name"]);
    payment->setColor(pay->fields["pay_color"]);
    payment->setSrcSign(pay->fields["src_sign"].toInt());
    payment->setDstSign(pay->fields["dst_sign"].toInt());

    if (mode == DEPOSIT) {
        payment->fields["payment_parent"] = view->fields["payment_id"];

    }
    if (debtType == Pay::CREATEDEBT){
        payment->fields["payment_debt"] = total.cents;
    }
    return true;
}

void PaymentDialog::setupTable(int sourceId, int type)
{
    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QTableView::item {margin-right: 10; margin-left: 10;}");

    PaymentModel *paymentModel = new PaymentModel(this);
    QSqlQuery &query = paymentModel->getRefreshQuery();

    if (mode == DEPOSIT) {
        query.prepare(Payment::allByParent);
    } else {
        query.prepare(Payment::allBySource);
    }

    query.bindValue(":id", sourceId);
    query.bindValue(":type", type);
    query.bindValue(":client", Client::currentRole);
    paymentModel->refresh();

    ui->table->setModel(paymentModel);
    //ui->table->hideColumn(2);
    ui->table->hideColumn(3);
}

void PaymentDialog::setupInfo(QString &info)
{
    ui->info->setPlainText(info);

    QTextCursor cursor(ui->info->textCursor());
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    ui->info->setTextCursor(cursor);
}

void PaymentDialog::retriveContact()
{
    int contactId;
    if (mode == DEPOSIT) {
        Payment *depositPayment = (Payment *)view;
        int type = depositPayment->getPayType();
        int id = view->fields["payment_field_id"].toInt();
        if (type == Pay::MATERIAL) {
            Material *material = new Material();
            material->retrive(id, false);
            contactId = material->fields["contact_id"].toInt();
            delete material;

        } else if (type == Pay::ORDER) {
            Order *order = new Order();
            order->retrive(id, false);
            contactId = order->fields["contact_id"].toInt();
            delete order;
        }

    } else {
         contactId = view->fields["contact_id"].toInt();
    }

    Entity *contact = ModelManager::getInstance()->getContactModel()->findEntity(contactId);
    int companyId = contact->fields["company_id"].toInt();
    Entity *company = ModelManager::getInstance()->getCompanyModel()->findEntity(companyId);

    contact->retrive(contactId, false);
    company->retrive(companyId, false);
}
