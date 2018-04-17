#include "PathFinder.h"
#include "../Vertex.h"
#include "../TriangleO.h"

using namespace deway;

PathFinder::PathFinder(std::vector<TriangleO *> * _navMesh)
{
    m_navMesh_ref = _navMesh;
    genNeigbours();
}
PathFinder::~PathFinder()
{
}

void PathFinder::genNeigbours()
{
    for(uint i = 0; i < m_navMesh_ref->size(); i++)//for each triangle 
    {
        TriangleO * t0 = (*m_navMesh_ref)[i];
        for(uint j = 0; j < 3; j++)//for each edge neighbour
        {
            uint ind0[2] = {j, j+1};
            if(ind0[1] > 2)
                ind0[1] = 0;
            
            Vertex * e0[2] = {t0->vertex[ind0[0]], t0->vertex[ind0[1]]};//our edge 
            //t0->nghbr[j];
            /////////////////////////////////////////////////////////////
            for(uint k = 0; k < m_navMesh_ref->size(); k++)
            {
                TriangleO * t1 = (*m_navMesh_ref)[k]; //potencial neighbour
                if(t1 == t0)//dont test against self
                    continue;
                
                for(uint l = 0; l < 3; l++)
                {
                    uint ind1[2] = {l, l+1};
                    if(ind1[1] > 2)
                        ind1[1] = 0;
                    
                    Vertex * e1[2] = {t1->vertex[ind1[0]], t1->vertex[ind1[1]]};//neighbours edge
                    if((e0[0] == e1[0] && e0[1] == e1[1]) || (e0[0] == e1[1] && e0[1] == e1[0]) )
                    {
                        t0->nghbr[j] = t1;
                        break;
                    }
                }
                if(t0->nghbr[j] != NULL)
                    break;
            }
            /////////////////////////////////////////////////////////////
        }
    }
}


