#ifndef PRINTTASKF_H
#define PRINTTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"
#include "entity/equip/equipment.h"

namespace Ui {
class PrintTaskF;
}

class PrintTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit PrintTaskF(QWidget *parent = 0, int processId = Process::PRINT);
    ~PrintTaskF();
    void entity2Form(Entity *task);

private:
    Ui::PrintTaskF *ui;
    void disableFocus();

private slots:
    void equipChanged();
    void stockChanged();
    void typeChanged(int type);
    void readyClicked();
};

#endif // PRINTTASKF_H
