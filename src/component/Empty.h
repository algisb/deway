#ifndef EMPTY_H_
#define EMPTY_H_
#include "Component.h"
#include "Entity.h"
#include "KePhys.h"
namespace kelp
{
    class Empty : public Component
    {
    public:
        float kekX;
        Entity * m_playerCameraEntity;
        
        Camera * m_playerCameraComp;
        kep::Vector3 lastFrameVel;
        
        Empty(Entity * _playerCamera);
        ~Empty();
        void init();
        void update();
        void render();
    };
};

#endif //EMPTY_H_
