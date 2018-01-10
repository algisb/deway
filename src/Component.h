
#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "Entity.h"


namespace kelp
{
    class Entity;
    class Component
    {
    public:
        Entity * m_owner;//owner is added by add component
        bool m_enabled;
        Component();
        virtual ~Component()=0;
        virtual void init()=0;
        virtual void update()=0;
        virtual void render();
        
    };
};

#endif // COMPONENT_H_
