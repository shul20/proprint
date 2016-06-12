#include "tab.h"
#include "ui_tab.h"
#include "utility/process.h"

#include <QDebug>

Tab::Tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab), currentOpen(NULL)
{
    ui->setupUi(this);
}

Tab::~Tab()
{
    delete ui;
}

void Tab::setData(QList<QWidget *> &data)
{
    QToolButton *button;

    for(int i = 0; i < data.size(); i++) {
        QWidget *widget = data.at(i);

        button = new QToolButton(this);
        button->setText(widget->objectName());
        button->setAutoRaise(true);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->setIcon(QIcon("://img/add-icon.png"));
        button->setStyleSheet(widget->styleSheet());
        map[button] = widget;
        ui->buttons->addWidget(button);
        ui->widget->addWidget(widget);
        widget->hide();
        /*ui->buttons->addItem(new QSpacerItem(5,20, QSizePolicy::Fixed,
                                                  QSizePolicy::Minimum));
*/
        connect(button, SIGNAL(clicked()), this, SLOT(pressButton()));
        connect(widget, SIGNAL(setCheck(bool)), this, SLOT(setCheck(bool)));
        connect(widget, SIGNAL(nameChanged()), this, SLOT(nameChanged()));

    }
    ui->buttons->addItem(new QSpacerItem(20,20, QSizePolicy::Expanding,
                                              QSizePolicy::Minimum));
}

void Tab::pressButton(){
    QToolButton *button = static_cast<QToolButton *>(sender());
    QWidget *widget = map.value(button);

    if (currentOpen != widget && currentOpen != NULL) {
        currentOpen->hide();
        QToolButton *button = map.key(currentOpen);
        button->setIcon(QIcon("://img/add-icon.png"));
    }

    if (widget->isHidden()) {
        widget->show();
        button->setIcon(QIcon("://img/expand_open-icon.png"));
        currentOpen = widget;
    } else {
        widget->hide();
        button->setIcon(QIcon("://img/add-icon.png"));
    }
}

void Tab::setCheck(bool check){
    QWidget *widget = static_cast<QWidget *>(sender());
    QToolButton *button = map.key(widget);

    if (check) {
        button->setStyleSheet("background-color: #FF7A7A");
    } else {
        button->setStyleSheet(widget->styleSheet());
    }
}

void Tab::nameChanged()
{
    QWidget *widget = static_cast<QWidget *>(sender());
    QToolButton *button = map.key(widget);
    button->setText(widget->objectName());
}
