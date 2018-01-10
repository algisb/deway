///
///  @file Entity.cpp
///  @brief Thing made up of smaller things

#include "Entity.h"
#include "component/Transform.h"
using namespace kelp;

Entity::Entity(World * _world, std::string _tag, Entity * _parent)
{
    m_world = _world;
    m_tag = _tag;
    m_parent = _parent;
    m_world->m_newEntities.push_back(this);
}
Entity::~Entity()
{
    for(int i = 0; i< m_components.size(); i++)
    {
        delete m_components[i];
        m_components[i]= NULL;
    }
    m_components.clear();
    m_world = NULL;
}

void Entity::init()
{
    m_transform = getComponent<Transform>();
    for(int i = 0; i< m_newComponents.size(); i++)
    {
        m_newComponents[i]->init();
        m_components.push_back(m_newComponents[i]);
    }
    m_newComponents.clear();
}

void Entity::update()
{
    for(int j = 0; j< m_exiledComponents.size(); j++)
    {
        for(int i = 0; i< m_components.size(); i++)
        {
            if(m_components[i] == m_exiledComponents[j])
            {
                delete m_components[i];
                m_components.erase(m_components.begin()+i);
                break;
            }
        }
    }
    m_exiledComponents.clear();
    
    init();//initialises object added at runtime
    for(int i = 0; i< m_components.size(); i++)
        if(m_components[i]->m_enabled)
            m_components[i]->update();
}

void Entity::render()
{
    for(int i = 0; i< m_components.size(); i++)
        if(m_components[i]->m_enabled)
            m_components[i]->render();
}

Component * Entity::addComponent(Component * _c)
{
    _c->m_owner = this;
    m_newComponents.push_back(_c);
    return _c;
}

int Entity::deleteComponent(Component* _c)
{
    for(int i = 0; i < m_components.size(); i++)
    {
        if(_c == m_components[i])
        {
            _c = NULL;
            delete m_components[i];
            m_components.erase(m_components.begin()+i);
            return 0;
        }
    }
    return 1;//the component does not exist
}
int Entity::deleteComponentNextFrame(Component* _c)
{
    for(int i = 0; i < m_components.size(); i++)
    {
        if(_c == m_components[i])
        {
            m_exiledComponents.push_back(_c);
            _c = NULL;
            return 0;//Component and will be deleted next frame
        }
    }
    return 1;//the component does not exist
}


