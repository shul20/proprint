#include "printerwidget.h"
#include "ui_printerwidget.h"
#include <QDebug>

int PrinterWidget::counter = -1;

PrinterWidget::PrinterWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::PrinterWidget)
{
    ui->setupUi(this);
    models = ModelManager::getInstance();


    modesF = new SortFilterModel(this);
    modesF->setSourceModel(models->getModeModel());
    modesF->setFilterRole(PmodeModel::equipIdRole);

    passesF = new SortFilterModel(this);
    passesF->setSourceModel(models->getPassModel());
    passesF->setFilterRole(PassModel::modeRole);

    profilesF = new SortFilterModel(this);
    profilesF->setSourceModel(models->getProfileModel());
    profilesF->setFilterRole(ProfileModel::passRole);

    ui->modes->setModel(modesF);
    connect(ui->modes->actionAdd, SIGNAL(triggered()),this, SLOT(addMode()));

    ui->passes->setModel(passesF);
    connect(ui->passes->actionAdd, SIGNAL(triggered()),this, SLOT(addPass()));
    ui->passes->blockSignals(true);

    ui->profiles->setModel(profilesF);
    connect(ui->profiles->actionAdd, SIGNAL(triggered()),this, SLOT(addProfile()));
    ui->profiles->blockSignals(true);
}

PrinterWidget::~PrinterWidget()
{
    delete ui;
}

void PrinterWidget::setPrinter(int printerId)
{
    modesF->setMatchValue(printerId);
}

void PrinterWidget::on_modes_clicked(const QModelIndex &index)
{
    Pmode *mode = (Pmode *) modesF->data(index, PPLib::entityRole).toInt();
    QString label = QString::fromUtf8("Пасы для режима %1 х %2 dpi")
            .arg(mode->fields["pmode_x"].toString())
            .arg(mode->fields["pmode_y"].toString());

    ui->passLabel->setText(label);

    passesF->setMatchValue(mode->getId());
    profilesF->setMatchValue(0);
    ui->passes->blockSignals(false);

    ui->profiles->blockSignals(true);
    ui->profileLabel->clear();
}

void PrinterWidget::on_passes_clicked(const QModelIndex &index)
{
    Pass *pass = (Pass *) passesF->data(index, PPLib::entityRole).toInt();
    QString label = QString::fromUtf8("Профили для %1 пасов")
            .arg(pass->fields["pass_name"].toString());

    profilesF->setMatchValue(pass->getId());
    ui->profileLabel->setText(label);
    ui->profiles->blockSignals(false);
}

void PrinterWidget::addMode()
{
    Pmode *pmode = new Pmode();
    pmode->fields["equipment_id"] = modesF->getMatchValue();
    pmode->fields["pmode_id"] = counter;
    modesF->setData(QModelIndex(), (int) pmode, PPLib::addRole);
    counter -= 1;
}

void PrinterWidget::addPass()
{
    Pass *pass = new Pass();
    pass->fields["pmode_id"] = passesF->getMatchValue();
    pass->fields["pass_id"] = counter;
    passesF->setData(QModelIndex(), (int) pass, PPLib::addRole);

    Profile *profile = new Profile();
    profile->fields["pass_id"] = counter;
    profile->fields["profile_name"] = QString::fromUtf8("без профиля");
    profilesF->setData(QModelIndex(), (int) profile, PPLib::addRole);

    counter -= 1;
}

void PrinterWidget::addProfile()
{
    Profile *profile = new Profile();
    profile->fields["pass_id"] = profilesF->getMatchValue();
    profilesF->setData(QModelIndex(), (int) profile, PPLib::addRole);
}

bool PrinterWidget::update(int printerId)
{
    for(int i = 0; i < modesF->rowCount(); i++){
        QModelIndex index = modesF->index(i, 0);
        Pmode *mode = (Pmode *) modesF->data(index, PPLib::entityRole).toInt();

        mode->fields["equipment_id"] = printerId;
        int oldId = mode->getId();
        if (oldId < 0) {
            mode->fields.remove("pmode_id");
        }
        if (!mode->update()){
            return false;
        }

        passesF->setMatchValue(oldId);

        if (!passesUpdate(mode->getId())){
            return false;
        }
    }
    return true;
}

bool PrinterWidget::passesUpdate(int modeId)
{
      for(int i = 0; i < passesF->rowCount(); i++){
        QModelIndex index = passesF->index(i, 0);
        Pass *pass = (Pass *) passesF->data(index, PPLib::entityRole).toInt();

        pass->fields["pmode_id"] = modeId;
        int oldId = pass->getId();
        if (oldId < 0) {
            pass->fields.remove("pass_id");
        }

        if (!pass->update()){
            return false;
        }

        profilesF->setMatchValue(oldId);

        if (!profilesUpdate(pass->getId())){
            return false;
        }
    }
    return true;
}

bool PrinterWidget::profilesUpdate(int passId)
{
    for(int i = 0; i < profilesF->rowCount(); i++){
        QModelIndex index = profilesF->index(i, 0);
        Profile *profile = (Profile *) profilesF->data(index, PPLib::entityRole).toInt();

        profile->fields["pass_id"] = passId;
        if (!profile->update()){
            return false;
        }
    }
    return true;
}

void PrinterWidget::exitNoUpdate()
{
    for(int row = modesF->rowCount() - 1; row >= 0; row--){
        int modeId = Entity::checkEntity(row, modesF);
        passesF->setMatchValue(modeId);

        for(int row = passesF->rowCount() - 1; row >= 0; row--){
            int passId = Entity::checkEntity(row, passesF);
            profilesF->setMatchValue(passId);

            for(int row = profilesF->rowCount() - 1; row >= 0; row--){
                Entity::checkEntity(row, profilesF);
            }
        }
    }
}
