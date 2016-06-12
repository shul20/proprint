#ifndef POCKETTASKF_H
#define POCKETTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"

namespace Ui {
class PocketTaskF;
}

class PocketTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit PocketTaskF(QWidget *parent = 0, int processId = Process::POCKET);
    ~PocketTaskF();
    void entity2Form(Entity *task);
    
private:
    Ui::PocketTaskF *ui;

    void disableFocus();
};

#endif // POCKETTASKF_H
