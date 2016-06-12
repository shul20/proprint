#ifndef EQUIPWIDGET_H
#define EQUIPWIDGET_H

#include <QWidget>
#include "models/sortfiltermodel.h"
#include "entity/equip/equipment.h"
#include "entity/equip/pmode.h"
#include "entity/equip/pass.h"
#include "entity/equip/profile.h"

namespace Ui {
class EquipWidget;
}

class EquipWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EquipWidget(QWidget *parent = 0);
    ~EquipWidget();
    void setup(bool isEquip, bool isMode, bool isPass, bool isProfile);

    void setEquipRole(int role, QVariant match = QVariant());

    void setEquip(int equipId);
    Equipment *getEquip() const;

    void setMode(int equipId, int modeId);
    Pmode *getMode() const;

    void setPass(int equipId, int modeId, int passId);
    Pass *getPass() const;

    void setProfile(int equipId, int modeId, int passId, int profileId);
    Profile *getProfile() const;

    void disableFocus();
    
private slots:
    void on_equip_currentIndexChanged(int index);

    void on_mode_currentIndexChanged(int index);

    void on_pass_currentIndexChanged(int index);

    void on_profile_currentIndexChanged(int index);

private:
    Ui::EquipWidget *ui;
    SortFilterModel *equipF, *modeF, *passF, *profileF;
    bool isEquip, isMode, isPass, isProfile;

signals:
    void equipChanged();
};

#endif // EQUIPWIDGET_H
