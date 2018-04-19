#ifndef EMPTY_H_
#define EMPTY_H_
#include "Component.h"
#include "Entity.h"
#include "KePhys.h"
#include "Agent.h"
#include "Mesh.h"
namespace kelp
{
    class Empty : public Component
    {
    public:
        float kekX;
        Entity * m_playerCameraEntity;
        
        Camera * m_playerCameraComp;
        kep::Vector3 lastFrameVel;
        
        KePhys * m_agentBody_ref;
        Agent * m_agent_ref;
        MeshGen * m_triGen_ref;
        
        Empty(Entity * _playerCamera, KePhys * _agentBody, Agent * _agent, MeshGen * _trigen);
        ~Empty();
        void init();
        void update();
        void render();
    };
};

#endif //EMPTY_H_
