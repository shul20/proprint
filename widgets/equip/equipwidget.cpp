#include "equipwidget.h"
#include "ui_equipwidget.h"
#include "models/modelmanager.h"
#include "utility/process.h"
#include "models/equip/pmodemodel.h"

EquipWidget::EquipWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EquipWidget),
    modeF(NULL), passF(NULL), profileF(NULL),
    isEquip(false), isMode(false), isPass(false), isProfile(false)
{
    ui->setupUi(this);

    equipF = new SortFilterModel(this);
    equipF->setSourceModel(ModelManager::getInstance()->getEquipModel());
}

EquipWidget::~EquipWidget()
{
    delete ui;
}

void EquipWidget::setup(bool isEquip, bool isMode, bool isPass, bool isProfile)
{
    this->isEquip = isEquip;
    this->isMode = isMode;
    this->isPass = isPass;
    this->isProfile = isProfile;

    ui->equip->setHidden(!isEquip);
    ui->mode->setHidden(!isMode);
    ui->pass->setHidden(!isPass);
    ui->profile->setHidden(!isProfile);

    ui->equip->setModel(equipF);
}

void EquipWidget::setEquipRole(int role, QVariant match)
{
    equipF->setFilterRole(role);
    equipF->setMatchValue(match);
}

void EquipWidget::setEquip(int equipId)
{
    if (equipId == 0) {
        ui->equip->setCurrentIndex(0);
        return;
    }

    int index = ui->equip->findData(equipId, PPLib::idRole);
    if (index < 0) {
        Equipment *equip = new Equipment();
        if (!equip->retrive(equipId, false)){
            delete equip;
            return;
        }
        equipF->setData(QModelIndex(), (int)equip, PPLib::addRole);
        ui->equip->setCurrentIndex(ui->equip->count());
        return;
    }
    ui->equip->setCurrentIndex(index);
}

Equipment *EquipWidget::getEquip() const
{
    int index = ui->equip->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Equipment *) ui->equip->itemData(index, PPLib::entityRole).toInt();
}

void EquipWidget::setMode(int equipId, int modeId)
{
    setEquip(equipId);
    if (modeId == 0) {
        ui->mode->setCurrentIndex(0);
        return;
    }
    int index = ui->mode->findData(modeId, PPLib::idRole);
    ui->mode->setCurrentIndex(index);
}

Pmode *EquipWidget::getMode() const
{
    int index = ui->mode->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Pmode *) ui->mode->itemData(index, PPLib::entityRole).toInt();
}

void EquipWidget::setPass(int equipId, int modeId, int passId)
{
    setMode(equipId, modeId);
    if (passId == 0) {
        ui->pass->setCurrentIndex(0);
        return;
    }
    int index = ui->pass->findData(passId, PPLib::idRole);
    ui->pass->setCurrentIndex(index);
}

Pass *EquipWidget::getPass() const
{
    int index = ui->pass->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Pass *) ui->pass->itemData(index, PPLib::entityRole).toInt();
}

void EquipWidget::setProfile(int equipId, int modeId, int passId, int profileId)
{
    setPass(equipId, modeId, passId);
    if (profileId == 0) {
        ui->profile->setCurrentIndex(0);
        return;
    }
    int index = ui->profile->findData(profileId, PPLib::idRole);
    ui->profile->setCurrentIndex(index);
}

Profile *EquipWidget::getProfile() const
{
    int index = ui->profile->currentIndex();
    if (index < 0) {
        return NULL;
    }
    return (Profile *) ui->profile->itemData(index, PPLib::entityRole).toInt();
}

void EquipWidget::disableFocus()
{
    ui->equip->setFocusPolicy(Qt::NoFocus);
    ui->mode->setFocusPolicy(Qt::NoFocus);
    ui->pass->setFocusPolicy(Qt::NoFocus);
    ui->profile->setFocusPolicy(Qt::NoFocus);
}

void EquipWidget::on_equip_currentIndexChanged(int index)
{
    if (index < 0 || !isMode) {
        return;
    }

    if (modeF == NULL) {
        modeF = new SortFilterModel(this);
        modeF->setSourceModel(ModelManager::getInstance()->getModeModel());
        ui->mode->setModel(modeF);
        modeF->setFilterRole(PmodeModel::equipIdRole);
    }
    Equipment *equip = (Equipment *) ui->equip->itemData(index, PPLib::entityRole).toInt();
    modeF->setMatchValue(equip->getId());

    if (modeF->rowCount() == 0) {
        if (passF != NULL) {
            passF->setMatchValue(0);
        }
        if (profileF != NULL) {
            profileF->setMatchValue(0);
        }
    }

    emit equipChanged();
}

void EquipWidget::on_mode_currentIndexChanged(int index)
{
    if (index < 0 || !isPass) {
        return;
    }

    if (passF == NULL) {
        passF = new SortFilterModel(this);
        passF->setSourceModel(ModelManager::getInstance()->getPassModel());
        ui->pass->setModel(passF);
        passF->setFilterRole(PassModel::modeRole);
    }
    passF->setMatchValue(ui->mode->itemData(index, PPLib::idRole));

    emit equipChanged();
}

void EquipWidget::on_pass_currentIndexChanged(int index)
{
    if (index < 0 || !isProfile) {
        return;
    }

    if (profileF == NULL) {
        profileF = new SortFilterModel(this);
        profileF->setSourceModel(ModelManager::getInstance()->getProfileModel());
        ui->profile->setModel(profileF);
        profileF->setFilterRole(ProfileModel::passRole);
    }
    profileF->setMatchValue(ui->pass->itemData(index, PPLib::idRole));

    emit equipChanged();
}

void EquipWidget::on_profile_currentIndexChanged(int index)
{
    emit equipChanged();
}
