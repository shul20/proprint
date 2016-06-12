#include "taskview.h"

const QString TaskView::table("task_view");

/*const char TaskView::allActive[] = "select * from task_view "
        "where task_status = :status and "
        "task_process in (%1)";
*/

const char TaskView::allByStatus[] = "select * from task_view "
                                   "where task_status = :status";



TaskView::TaskView():Entity()
{
    Entity::tableName = &table;
    Entity::tableView = &table;
}

TaskView::TaskView(TaskView *other):Entity(other)
{}

bool TaskView::update()
{
    return true;
}
