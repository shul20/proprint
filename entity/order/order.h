#ifndef ORDER_H
#define ORDER_H

#include <QStringList>
#include "entity/entity.h"

class Order : public Entity
{
private:
    static const QString table, tableView;

public:
    Order();
    Order(Order *other);

    static bool setPriority(int orderId, int priority);
    static bool setStatus(int orderId, int status);
    static void openOrder(QAbstractItemModel *model, int mode, Entity *orderView);
    static void openProcess(QAbstractItemModel *model, Entity *orderView);

    static const char viewByStatus[], viewArch[], searchStoryArch[];
    static const QStringList statuses, packs;
    static const enum {
        DELETE = 0,  PREPARE = 1, TOWORK = 2, PAUSE = 3,
        PROCESS = 4, POSTPROCESS = 5, READY = 6, ARCHIVE = 7} statusEnum;

    static const enum {
        ADD = 0,  EDIT = 1, VIEW = 2} modesEnum;

    static const QStringList deliveries;
    static const enum {SELF = 0, OFFICE = 1, TAXI = 2, NEWPOST = 3} deliveryType;
    static const enum {ROLL = 0, TUBE = 1, OTHER = 2} packEnum;

};

#endif // ORDER_H
