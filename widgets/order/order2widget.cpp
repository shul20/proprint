#include "order2widget.h"
#include "ui_order2widget.h"
#include "order2/deliverywidget.h"
#include "order2/packwidget.h"
#include "order2/storywidget.h"

Order2Widget::Order2Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Order2Widget), process(NULL)
{
    ui->setupUi(this);
    widgets.append(new DeliveryWidget());
    widgets.append(new PackWidget());
    widgets.append(new StoryWidget());

    rightsWidget = new RightsWidget();
    widgets.append(rightsWidget);
}

Order2Widget::~Order2Widget()
{
    delete ui;

}

void Order2Widget::setOrder(Entity *order, int mode, bool isCopy)
{
    if (mode == PROCESS) {
        if (process == NULL) {
            process = new ProcessInfoW();
        }
        widgets.append(process);

    } else if (mode == CREATE && process != NULL){
        widgets.removeOne(process);
    }
    ui->tab->setData((QList<QWidget *> &)widgets);

    foreach (Order2IWidget * widget, widgets) {
        widget->setMode(mode);
        widget->entity2Form(order);
    }
}

bool Order2Widget::form2Entity(Entity *order)
{
    foreach (Order2IWidget * widget, widgets) {
        if (!widget->form2Entity(order)) {
            return false;
        }
    }
    return true;
}

void Order2Widget::clear()
{
}

void Order2Widget::setReadOnly()
{
    foreach (Order2IWidget * widget, widgets) {
        widget->setReadOnly();
    }
}

void Order2Widget::setReadOnlyAll()
{
    setReadOnly();
    rightsWidget->setWidgetReadOnly();
    if (process != NULL) {
        process->setWidgetReadOnly();
    }
}
