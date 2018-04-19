///
///  @file World.cpp
///  @brief all worlds are derived from this class

#include "World.h"
#include "component/RenderLine.h"
#include "Core.h"
using namespace kelp;

World::World(Core * _core)
{
    m_renderCamera = NULL;
    m_core = _core;
    int m_numLights = 0;
    m_physWorld = new kep::World();
    m_init = false;
}
World::~World()
{
    for(int i = 0; i< m_entities.size(); i++)
        delete m_entities[i];
    m_entities.clear();
    delete m_physWorld;
}

void World::update()
{
    for(int j = 0; j< m_exiledEntities.size(); j++)
    {
        for(int i = 0; i< m_entities.size(); i++)
        {
            if(m_entities[i] == m_exiledEntities[j])
            {
                delete m_entities[i];
                m_entities.erase(m_entities.begin()+i);
                break;
            }
        }
    }
    m_exiledEntities.clear();
    
    
    for(int i = 0; i< m_newEntities.size(); i++)
    {
        m_newEntities[i]->init();
        m_entities.push_back(m_newEntities[i]);
    }
    m_newEntities.clear();
    if(!m_init)
    {
        m_init = true;
        initW();
    }
    
    for(int i = 0; i< m_entities.size(); i++)
        m_entities[i]->update();
    m_physWorld->update(Time::s_deltaT);
    updateW();
    
    for(int i = 0; i< m_entities.size(); i++)
        m_entities[i]->render();
    RenderLine::renderLines(m_core->m_shaderMinimal, m_renderCamera);
    RenderLine2::renderLines(m_core->m_shaderMinimal, m_renderCamera);
}

int World::deleteEntity(Entity * _entity)
{
    for(int i = 0; i< m_entities.size(); i++)
    {
        if(m_entities[i] == _entity)
        {
            _entity = NULL;
            delete m_entities[i];
            m_entities.erase(m_entities.begin()+i);
            return 0;//Entity found and will be deleted next frame
        }
    }
    return 1;//Specified entity does not exist
}

int World::deleteEntityNextFrame(Entity* _entity)
{
    for(int i = 0; i< m_entities.size(); i++)
    {
        if(m_entities[i] == _entity)
        {
            m_exiledEntities.push_back(_entity);
            _entity = NULL;
            return 0;//Entity found and will be deleted next frame
        }
    }
    return 1;//Specified entity does not exist
}

