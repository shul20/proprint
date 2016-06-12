#include "contactdialog.h"
#include "ui_contactdialog.h"
#include "proprintlib.h"
#include "models/sortfiltermodel.h"

ContactDialog::ContactDialog(QWidget *parent) :QDialog(parent),
    ui(new Ui::ContactDialog){

    ui->setupUi(this);
}

ContactDialog::~ContactDialog(){
    delete ui;
}

void ContactDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
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
        ui->saveContactButton->setText(QString::fromUtf8("Добавить"));
        setWindowTitle(QString::fromUtf8("Добавить контакт"));

    } else if (mode == Qt::EditRole) {
        view = (Entity *) model->data(index, PPLib::entityRole).toInt();
        entity2Form(view);
        ui->saveContactButton->setText(QString::fromUtf8("Изменить"));
        setWindowTitle(QString::fromUtf8("Редактирование контакта"));
    }
}

void ContactDialog::clear()
{
    ui->name->clear();
    ui->surname->clear();
    ui->email->clear();
    ui->tel->clear();
}


void ContactDialog::on_saveContactButton_clicked(){
    Contact *saveContact = NULL;

    if (mode == PPLib::addRole) {
        saveContact = new Contact();
        SortFilterModel *modelF = (SortFilterModel *) model;
        saveContact->fields["company_id"] = modelF->getMatchValue();
        saveContact->fields["contact_debt"] = 0;

    } else if (mode == Qt::EditRole) {
        saveContact = new Contact((Contact *)view);
    }

    if (!form2Entity(saveContact)) {

        delete saveContact;
        return;
    }

    model->setData(index, (int) saveContact, mode);
    QDialog::accept();
}

void ContactDialog::entity2Form(Entity *contact)
{
    ui->name->setText(contact->fields["contact_name"].toString());
    ui->surname->setText(contact->fields["contact_surname"].toString());
    ui->email->setText(contact->fields["contact_email"].toString());
    ui->tel->appendPlainText(contact->fields["contact_tel"].toString());
    ui->tel->moveCursor(QTextCursor::Start);
}

bool ContactDialog::form2Entity(Entity *entity)
{
    QString name = ui->name->text();
    QString surname = ui->surname->text();
    QString email = ui->email->text();
    QString tel = ui->tel->toPlainText();

    if (name.length() == 0 || surname.length() == 0 ) {
        PPLib::fillFormMessage();
        return false;
    }

    entity->fields["contact_name"] = name;
    entity->fields["contact_surname"] = surname;
    entity->fields["contact_email"] = email;
    entity->fields["contact_tel"] = tel;
    return true;
}


void ContactDialog::on_exitContactButton_clicked()
{
    QDialog::reject();
}
