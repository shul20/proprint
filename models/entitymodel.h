#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include "proprintlib.h"
#include "entity/entity.h"

class Entity;

class EntityModel : public QAbstractTableModel
{
    Q_OBJECT

protected:
    const QStringList *entityHeader;
    QList<Entity *> entities;
    QList<Entity *> delEntities;
    QSqlQuery refreshQuery;

public:
    explicit EntityModel(QObject *parent = 0);
    virtual ~EntityModel();

    Entity *getEntity(int row) const;
    Entity *findEntity(int id) const;

    void appendEntity(Entity *entity);
    virtual Entity *createEntity() const = 0;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                                     int role = Qt::DisplayRole) const;

    bool insertRows(int position, int rows, const QModelIndex);
    bool removeRows(int position, int rows, const QModelIndex);
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex & index) const ;
    void clearEntities();
    QSqlQuery &getRefreshQuery();
    bool delRemoved();
    void unDelete();

public slots:
    virtual void refresh();

};

#endif // ENTITYMODEL_H
