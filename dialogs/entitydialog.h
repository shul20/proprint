#ifndef ENTITYDIALOG_H
#define ENTITYDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

#include "entity/entity.h"

class EntityDialog : public QDialog
{
    Q_OBJECT
protected:
    QAbstractItemModel *model;
    QModelIndex index;
    Entity *entityTemplate, *view;
    int mode;
    virtual bool form2Entity(Entity *entity) = 0;
    virtual bool update(Entity* entity, const QModelIndex &index, int role);
    void move2Center();

public:
    explicit EntityDialog(QWidget *parent);
    virtual ~EntityDialog();
    virtual void set(QAbstractItemModel *model, int mode, const QModelIndex &index);
    void setModel(QAbstractItemModel *model);
    void setIndex(const QModelIndex &index);
    void setEntityTemplate(Entity *entity);
    
protected slots:
    void exit();
    virtual void hideEvent(QHideEvent *);
};

#endif // ENTITYDIALOG_H
