#ifndef INFOTASKF_H
#define INFOTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"

namespace Ui {
class InfoTaskF;
}

class InfoTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit InfoTaskF(QWidget *parent = 0, int processId = Process::FINISHING);
    ~InfoTaskF();
    void entity2Form(Entity *task);

private:
    Ui::InfoTaskF *ui;
    void disableFocus();
};

#endif // INFOTASKF_H
