#ifndef STORYWIDGET_H
#define STORYWIDGET_H

#include "widgets/order/order2/order2iwidget.h"

namespace Ui {
class StoryWidget;
}

class StoryWidget : public Order2IWidget
{
    Q_OBJECT
    
public:
    explicit StoryWidget(QWidget *parent = 0);
    ~StoryWidget();
    void clear();
    void entity2Form(Entity *order);
    bool form2Entity(Entity *order);
    void setReadOnly();
    
private:
    Ui::StoryWidget *ui;

private slots:
    void on_markInfo_textChanged();
    void on_storyInfo_textChanged();
};

#endif // STORYWIDGET_H
