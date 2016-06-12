#ifndef MSGVIEW_H
#define MSGVIEW_H

#include <QWidget>
#include <QDate>
#include <QMenu>
#include "models/messagemodel.h"

namespace Ui {
class MsgView;
}

class MsgView : public QWidget
{
    Q_OBJECT
    
public:
    explicit MsgView(QWidget *parent = 0);
    ~MsgView();
    

private:
    Ui::MsgView *ui;
    MessageModel *model;
    QMenu *menu;

    void appendMenu();
    void refresh(QDate from, QDate till);
    Entity *currentEntity();

private slots:
    void on_table_customContextMenuRequested(const QPoint &pos);
    void on_dateFrom_dateChanged(const QDate &date);
    void on_dateTill_dateChanged(const QDate &date);
    void addMessage();
    void delMessage();
    void editMessage(QModelIndex index);
};

#endif // MSGVIEW_H
