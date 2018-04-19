#ifndef AGENT_H_
#define AGENT_H_

#include "Component.h"
#include "kep/Core.h"
#include "Config.h"
#include "Transform.h"

namespace deway
{
    class PathFinder;
};
namespace kelp
{
    
    class Agent : public Component
    {
    public:
        Transform * m_transform;
        deway::PathFinder * m_pathFinder;
        Agent(deway::PathFinder * _pathFinder);
        ~Agent();
        
        void init();
        void update();
        void render();
    };
};

#endif //AGENT_H_
