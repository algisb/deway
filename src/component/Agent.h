#ifndef AGENT_H_
#define AGENT_H_

#include "Component.h"
#include "kep/Core.h"
#include "Config.h"
#include "Transform.h"
#include "KePhys.h"
#include "../deway/typedefs.h"
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
        KePhys * m_kephys;
        deway::PathFinder * m_pathFinder;
        
        
        std::vector<kep::Vector3> m_path;
        deway::uint m_pathProgress;
        
        
        Agent(deway::PathFinder * _pathFinder);
        ~Agent();
        
        void init();
        void update();
        void render();
        
        void tracePath(deway::Vertex * _endVertex);
        bool findVertex(deway::Vertex * _vertToFind, std::vector<deway::Vertex*> * _container);
        void clearGraph();
        
        int genPath(deway::Loc * _finish);
        
        bool atPos(kep::Vector3 _pos);

    };
};

#endif //AGENT_H_
