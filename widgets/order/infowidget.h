#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QFile>
#include "entity/entity.h"

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit InfoWidget(QWidget *parent = 0);
    ~InfoWidget();

public slots:
    void setOrder(const QModelIndex &index);
    
private slots:
    void on_table_clicked(const QModelIndex &index);

private:
    Ui::InfoWidget *ui;
    QFile html;

    void setupInfo(Entity *order);
    void setupFlow(Entity *order, Entity *orderView);
    void setupFin(Entity *order, Entity *orderView);
};

#endif // INFOWIDGET_H
