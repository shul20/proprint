#ifndef LAMINATTASKF_H
#define LAMINATTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"
#include "entity/equip/equipment.h"

namespace Ui {
class LaminatTaskF;
}

class LaminatTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit LaminatTaskF(QWidget *parent = 0, int processId = Process::LAMINAT);
    ~LaminatTaskF();
    void entity2Form(Entity *task);
    
private:
    Ui::LaminatTaskF *ui;
    void disableFocus();

private slots:
    void equipChanged();
    void stockChanged();
    void readyClicked();
};

#endif // LAMINATTASKF_H
