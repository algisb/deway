#ifndef PATHFINDER_H_
#define PATHFINDER_H_
#include <vector>
#include "../typedefs.h"
namespace deway
{
    class TriangleO;
    class PathFinder 
    {
    public:
        std::vector<TriangleO*> * m_navMesh_ref;
        
        PathFinder(std::vector<TriangleO*> * _navMesh);
        ~PathFinder();
        void genNeigbours();
    };
};

#endif
