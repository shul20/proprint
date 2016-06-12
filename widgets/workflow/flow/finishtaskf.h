#ifndef FINISHTASKF_H
#define FINISHTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"

namespace Ui {
class FinishTaskF;
}

class FinishTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit FinishTaskF(QWidget *parent = 0, int processId = Process::FINISHING);
    ~FinishTaskF();
    void entity2Form(Entity *task);
    
private:
    Ui::FinishTaskF *ui;
    void disableFocus();

};

#endif // FINISHTASKF_H
