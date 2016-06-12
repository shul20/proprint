#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QStringList>
#include "entity/entity.h"

class TaskView : public Entity
{
private:
    static const QString table;

public:
    TaskView();
    TaskView(TaskView *other);

    bool update();

    static const char allByStatus[];
};

#endif // TASKVIEW_H
