#include "storywidget.h"
#include "ui_storywidget.h"
#include <QMessageBox>

StoryWidget::StoryWidget(QWidget *parent) :
    Order2IWidget(parent),
    ui(new Ui::StoryWidget)
{
    ui->setupUi(this);
    setObjectName(QString::fromUtf8("Примечание/Сюжет"));

    QString color = QString("background: rgba(255,85,0,45)");
    setStyleSheet(QString("QToolButton {%1} QFrame {%1}") .arg(color));
}

StoryWidget::~StoryWidget()
{
    delete ui;
}

void StoryWidget::clear()
{
    ui->markInfo->clear();
    ui->storyInfo->clear();
}

void StoryWidget::entity2Form(Entity *order)
{
    ui->markInfo->setPlainText(order->fields["ordr_mark"].toString());
    ui->storyInfo->setPlainText(order->fields["ordr_story"].toString());
}

bool StoryWidget::form2Entity(Entity *order)
{
    if (isReadOnly) {
        return true;
    }
    QString story = ui->storyInfo->toPlainText();

#ifndef QT_DEBUG
    if (story.length() == 0) {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
               QString::fromUtf8("Укажите сюжет"), QMessageBox::Ok);
        return false;
    }
#endif

    order->fields["ordr_mark"] = ui->markInfo->toPlainText();
    order->fields["ordr_story"] = story;

    return true;
}

void StoryWidget::setReadOnly()
{
    isReadOnly = true;
    ui->markInfo->setFocusPolicy(Qt::NoFocus);
    ui->storyInfo->setFocusPolicy(Qt::NoFocus);
}

void StoryWidget::on_markInfo_textChanged()
{
    checkText(ui->markInfo->toPlainText());
}

void StoryWidget::on_storyInfo_textChanged()
{
    checkText(ui->storyInfo->toPlainText());
}
