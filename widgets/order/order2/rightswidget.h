#ifndef RIGHTSWIDGET_H
#define RIGHTSWIDGET_H

#include "widgets/order/order2/order2iwidget.h"
#include "models/sortfiltermodel.h"

namespace Ui {
class RightsWidget;
}

class RightsWidget : public Order2IWidget
{
    Q_OBJECT
    
public:
    explicit RightsWidget(QWidget *parent = 0);
    ~RightsWidget();
    void clear();
    void entity2Form(Entity *order);
    bool form2Entity(Entity *order);
    void setReadOnly();
    void setWidgetReadOnly();
    
private slots:
    void on_managerBox_clicked(bool checked);
    void on_checkForAll_clicked(bool checked);

private:
    Ui::RightsWidget *ui;
    SortFilterModel *managerF, *operatorF;
    Entity *order;
};

#endif // RIGHTSWIDGET_H
