#include "entityview.h"

EntityView::EntityView():Entity()
{}

EntityView::EntityView(EntityView *other):Entity(other)
{}

bool EntityView::update()
{
    return true;
}
