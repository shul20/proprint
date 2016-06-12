#include "entitymodel.h"
#include "proprintlib.h"


EntityModel::EntityModel(QObject *parent)
    :QAbstractTableModel(parent)
    {}

EntityModel::~EntityModel(){
    clearEntities();
}

QSqlQuery &EntityModel::getRefreshQuery()
{
    return refreshQuery;
}

bool EntityModel::delRemoved()
{
    foreach (Entity *entity, delEntities){
        if (!entity->del()){
            return false;
        }
        delete entity;
    }
    delEntities.clear();
    return true;
}

void EntityModel::unDelete()
{
    foreach (Entity *entity, delEntities){
        if (entity->getId() == 0){
            delete entity;
        } else {
            entities.append(entity);
        }

    }
    delEntities.clear();
}

Entity *EntityModel::getEntity(int row) const
{
    return entities.at(row);
}

Entity *EntityModel::findEntity(int id) const
{
    QModelIndexList indexes = match(this->index(0,0), PPLib::idRole, id);
    if (indexes.size() == 0){
        return NULL;
    }
    return (Entity *) data(indexes.first(), PPLib::entityRole).toInt();
}

void EntityModel::appendEntity(Entity *entity)
{
    entities.append(entity);
}

int EntityModel::rowCount(const QModelIndex &parent) const
{
    return entities.size();
}

int EntityModel::columnCount(const QModelIndex &parent) const
{
    return entityHeader->size();
}

QVariant EntityModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    Entity *entity = entities.at(row);

    if (role == PPLib::entityRole) {
        return (int)entity;
    }
    if (role == PPLib::idRole) {
        return entity->getId();
    }

    return QVariant();
}

QVariant EntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return entityHeader->at(section);

    } else {
        return QString("%1").arg( section + 1 );
    }
}

void EntityModel::clearEntities()
{
    foreach (Entity *entity, entities) {
        delete entity;
    }
    entities.clear();

    foreach (Entity *entity, delEntities) {
        delete entity;
    }
    delEntities.clear();
}

bool EntityModel::insertRows(int position, int rows, const QModelIndex)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);
    endInsertRows();
    return true;
}

bool EntityModel::removeRows(int position, int rows, const QModelIndex)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    endRemoveRows();
    return true;
}

bool EntityModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == PPLib::addRole){
        if (value.isValid()) {
            entities.append((Entity *)value.toInt());
        } else {
            Entity *entity = createEntity();
            entities.append(entity);
        }
        insertRows(0, 1, QModelIndex());
        reset();

    } else if (role == Qt::EditRole) {
        int row = index.row();
        Entity *entity = entities.at(row);
        Entity *editEntity = (Entity *)value.toInt();

        entities.replace(row, editEntity);
        delete entity;
        entity = NULL;
        reset();

    } else if (role == PPLib::refreshRole){
        refresh();

    } else if (role == PPLib::delRole) {
        int row = index.row();
        Entity *entity = entities.at(row);

        if (value.toBool()) {
            if (!entity->del()) {
                return false;
            }
            entities.removeOne(entity);
            delete entity;
            entity = NULL;

        } else {
            delEntities.append(entity);
            entities.removeOne(entity);
        }
        removeRows(row, 1, QModelIndex());
    }
    return true;
}

Qt::ItemFlags EntityModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

void EntityModel::refresh()
{
    if (!refreshQuery.exec()){
        return;
    }
    clearEntities();

    for(;refreshQuery.next();){
        Entity *entity = createEntity();
        entity->setFields(refreshQuery.record());
        entities.append(entity);
    }
    reset();
}
