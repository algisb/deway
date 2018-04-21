#ifndef EMPTY_H_
#define EMPTY_H_
#include "Component.h"
#include "Entity.h"
#include "KePhys.h"
#include "Agent.h"
#include "Mesh.h"

namespace deway 
{
    class Ray;
    class Loc;
};
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
        MeshGenLine * m_testLine_ref;
        
        Empty(Entity * _playerCamera, KePhys * _agentBody, Agent * _agent, MeshGen * _trigen, MeshGenLine * _testLine);
        ~Empty();
        void init();
        void update();
        void render();
        
        deway::Ray genMouseRay();
        deway::Loc getMouseRayLoc(deway::Ray _mouseRay);
        
        
        
        
    };
};

#endif //EMPTY_H_
