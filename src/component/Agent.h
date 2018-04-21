#ifndef AGENT_H_
#define AGENT_H_

#include "Component.h"
#include "kep/Core.h"
#include "Config.h"
#include "Transform.h"

namespace deway
{
    class PathFinder;
    class Loc;
    class Vertex;
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
        
        void tracePath(deway::Vertex * _endVertex, std::vector<kep::Vector3> * o_path);
        
        bool findVertex(deway::Vertex * _vertToFind, std::vector<deway::Vertex*> * _container);
        int genPath(deway::Loc * _finish, std::vector<kep::Vector3> * o_path);
    };
};

#endif //AGENT_H_
