#ifndef ENTITYTABLE_H
#define ENTITYTABLE_H

#include <QTableView>
#include <QMenu>
#include "entity/entity.h"
#include "dialogs/entitydialog.h"

class EntityTable : public QTableView
{
    Q_OBJECT
private:
    QMenu *menu;
    EntityDialog *dialog;

    void setupMenu();

public:
    QMenu *getMenu();
    QAction *actionAdd, *actionEdit, *actionRefresh, *actionDel;

    explicit EntityTable(QWidget *parent = 0);
    Entity *currentEntity();
    void setDialog(EntityDialog *dialog);

    void setSortingEnabled(bool enable);
    static void setup(QTableView *table);
    void setModel(QAbstractItemModel *model);

public slots:


public slots:
    virtual void refresh();
    virtual void del();
    void customContextMenu(const QPoint &pos);



    
};

#endif // ENTITYTABLE_H
