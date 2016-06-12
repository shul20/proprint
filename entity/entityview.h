#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H

#include "entity.h"

class EntityView : public Entity
{
public:
    EntityView();
    EntityView(EntityView *other);

    bool update();
};

#endif // ENTITYVIEW_H
