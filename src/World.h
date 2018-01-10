#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include "Entity.h"
#include "kep/World.h"

//Made up of entities
namespace kelp
{
    class Core;
    class Component;
    
    class Entity;
    class Camera;
    class Light;
    class World
    {
    public:
        bool m_init;
        Core * m_core;
        Camera * m_renderCamera;
        kep::World * m_physWorld;//Physics world
        std::vector<Light*> m_lights;
        std::vector<Entity*> m_entities;
        std::vector<Entity*> m_newEntities;//entities to be added ie. initializer not called yet-- BCUZ ENTITY CANT BE ADDED MID LOOP
        std::vector<Entity*> m_exiledEntities;//entities to be deleted on the next frame
        World(Core * _core);
        virtual ~World()=0;
        virtual void initW()=0;
        virtual void updateW()=0;
        void update();
        int deleteEntity(Entity * _entity);
        int deleteEntityNextFrame(Entity * _entity);
    };
};
#endif // WORLD_H_
