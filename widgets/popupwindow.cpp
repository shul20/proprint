#include "popupwindow.h"
#include "ui_popupwindow.h"
#include <QDebug>

PopUpWindow::PopUpWindow(QWidget *parent, const QString &text) :
    QWidget(parent),
    ui(new Ui::PopUpWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    ui->label->setText(text);
}

PopUpWindow::~PopUpWindow()
{
    delete ui;
}

