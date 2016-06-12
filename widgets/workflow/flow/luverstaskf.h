#ifndef LUVERSTASKF_H
#define LUVERSTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"
#include "entity/equip/equipment.h"

namespace Ui {
class LuversTaskF;
}

class LuversTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit LuversTaskF(QWidget *parent = 0, int processId = Process::LUVERS);
    ~LuversTaskF();
    void entity2Form(Entity *task);

private:
    Ui::LuversTaskF *ui;
    void disableFocus();

private slots:
    void stockChanged();
};

#endif // LUVERSTASKF_H
