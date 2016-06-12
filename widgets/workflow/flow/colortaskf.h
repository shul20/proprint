#ifndef COLORTASKF_H
#define COLORTASKF_H

#include "widgets/workflow/flow/flowwidget.h"
#include "entity/entity.h"
#include "utility/process.h"
#include "entity/equip/equipment.h"

namespace Ui {
class ColorTaskF;
}

class ColorTaskF : public FlowWidget
{
    Q_OBJECT
    
public:
    explicit ColorTaskF(QWidget *parent = 0, int processId = Process::COLORPROOF);
    ~ColorTaskF();
    void entity2Form(Entity *task);

private:
    Ui::ColorTaskF *ui;

    void disableFocus();

private slots:
    void equipChanged();
    void stockChanged();
    void typeChanged(int type);
    void readyClicked();
};

#endif // COLORTASKF_H
