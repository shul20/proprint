#ifndef MOUNTTASKF_H
#define MOUNTTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"
#include "entity/equip/equipment.h"

namespace Ui {
class MountTaskF;
}

class MountTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit MountTaskF(QWidget *parent = 0, int processId = Process::MOUNT);
    ~MountTaskF();
    void entity2Form(Entity *task);
    
private:
    Ui::MountTaskF *ui;

    void disableFocus();

private slots:
    void readyClicked();
    void stockChanged();
};

#endif // MOUNTTASKF_H
