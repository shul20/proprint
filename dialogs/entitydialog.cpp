#include "entitydialog.h"
#include <QApplication>
#include <QDesktopWidget>

EntityDialog::EntityDialog(QWidget *parent) :
    QDialog(parent), entityTemplate(NULL), view(NULL)
{}

EntityDialog::~EntityDialog()
{}

void EntityDialog::set(QAbstractItemModel *model, int mode, const QModelIndex &index)
{
    this->model = model;
    this->index = index;
    this->mode = mode;

}

void EntityDialog::setModel(QAbstractItemModel *model)
{
    this->model = model;
}

void EntityDialog::setIndex(const QModelIndex &index)
{
    this->index = index;
}

void EntityDialog::setEntityTemplate(Entity *entity)
{
    entityTemplate = entity;
}

void EntityDialog::exit()
{
    QDialog::reject();
}

void EntityDialog::hideEvent(QHideEvent *)
{
    entityTemplate = NULL;
}


bool EntityDialog::update(Entity *entity, const QModelIndex &index, int role)
{
    if (!form2Entity(entity) || !entity->update()) {
        return false;
    }

    model->setData(index, QVariant((int) entity), role);
    return true;
}

void EntityDialog::move2Center()
{
    const QRect screen = QApplication::desktop()->screenGeometry();
    move( screen.center() - rect().center() );

}
