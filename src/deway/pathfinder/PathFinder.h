#ifndef PATHFINDER_H_
#define PATHFINDER_H_
#include <vector>
#include "../typedefs.h"
#include "Core.h"
namespace deway
{
    class Loc;
    class Ray;
    class TriangleO;
    class Vertex;
    class PathFinder 
    {
    public:
        std::vector<Vertex*> * m_verts_ref;
        std::vector<TriangleO*> * m_navMesh_ref;
        std::vector<TriangleO*> m_navMeshSort; //Nav mesh triangle list sorted by Y height in decending order
        
        PathFinder(std::vector<TriangleO*> * _navMesh, std::vector<Vertex*> * _verts);
        ~PathFinder();
        void genNeigbours();
        void genNormals();
        void sortMesh();
        
        static int rayTriangleMT97(Ray * _ray, TriangleO * _triangle,  kep::Vector3 * o_point);
        
        int getLoc(Loc * o_loc, kep::Vector3 _pos);
    };
};

#endif
