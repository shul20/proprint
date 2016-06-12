#ifndef SIZEWIDGET_H
#define SIZEWIDGET_H

#include <QWidget>
#include "utility/size.h"
#include "entity/entity.h"

namespace Ui {
class SizeWidget;
}

class SizeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SizeWidget(QWidget *parent = 0);
    ~SizeWidget();
    void clear();
    void entity2Form(Entity *entity);
    bool form2Entity(Entity *entity);

    int getAmount();
    int getWidth();
    int getHeight();
    Size &getSize();
    
private slots:
    void on_width_valueChanged(int arg1);

    void on_height_valueChanged(int arg1);

    void on_amount_valueChanged(int arg1);

private:
    Ui::SizeWidget *ui;
    int width, height, amount;
    Size workSize;

    void calculateSize();

signals:
    void sizeChanged();
};

#endif // SIZEWIDGET_H
