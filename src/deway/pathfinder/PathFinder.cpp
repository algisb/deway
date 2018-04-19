#include "PathFinder.h"
#include "../Vertex.h"
#include "../TriangleO.h"
#include "Ray.h"
#include "Loc.h"
using namespace deway;

PathFinder::PathFinder(std::vector<TriangleO *> * _navMesh)
{
    m_navMesh_ref = _navMesh;
    genNeigbours();
    sortMesh();
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

void PathFinder::genNormals()
{
    for(uint i = 0; i<m_navMesh_ref->size(); i++)
    {
        TriangleO * t = (*m_navMesh_ref)[i];
        kep::Vector3 v1 = t->vertex[1]->pos - t->vertex[0]->pos;
        kep::Vector3 v2 = t->vertex[2]->pos - t->vertex[0]->pos;
        t->normal = kep::cross(v1,v2).normalized();
    }
}

void PathFinder::sortMesh()
{
    for(uint i = 0; i<m_navMesh_ref->size(); i++)
        m_navMeshSort.push_back((*m_navMesh_ref)[i]);
    
    while(1)
    {
        bool isSorted = true;
        for(uint i = 0; i<m_navMeshSort.size()-1; i++)
        {
            float y0 = (m_navMeshSort[i]->vertex[0]->pos.y + m_navMeshSort[i]->vertex[1]->pos.y + m_navMeshSort[i]->vertex[2]->pos.y)/3.0f;
            float y1 = (m_navMeshSort[i+1]->vertex[0]->pos.y + m_navMeshSort[i+1]->vertex[1]->pos.y + m_navMeshSort[i+1]->vertex[2]->pos.y)/3.0f;
            
            if(y1 > y0)
            {
                TriangleO * tmpTri = m_navMeshSort[i];
                m_navMeshSort[i] = m_navMeshSort[i+1];
                m_navMeshSort[i+1] = tmpTri;
                isSorted = false;
            }
        }
        if(isSorted)
            break;
    }
}

int PathFinder::rayTriangleMT97(Ray * _ray, TriangleO * _triangle,  kep::Vector3 * o_point)// 64 flops
{
    kep::Vector3 edge1, edge2, pvec, tvec, qvec;
    float det, inv_det, u, v, w, t;
    edge1 = _triangle->vertex[1]->pos - _triangle->vertex[0]->pos;// 3 flops
    edge2 = _triangle->vertex[2]->pos - _triangle->vertex[0]->pos;// 3 flops
    
    pvec  = kep::cross(_ray->d, edge2);// 9 flops
    
    det = kep::dot(edge1, pvec);// 5 flops
    if(det == 0.0f)
        return 0;
    
    inv_det = 1.0f/det;// 1 flops
    
    tvec = _ray->s - _triangle->vertex[0]->pos;// 3 flops
    
    u = kep::dot(tvec, pvec) * inv_det;// 8 flops
    if(u < 0.0f || u > 1.0f)
        return 0;
    
    qvec = kep::cross(tvec, edge1);//9 flops
    
    v = kep::dot(_ray->d, qvec) * inv_det;// 8 flops
    if(v < 0.0f || v > 1.0f)
        return 0;
    
    w = u + v;//1 flop
    if(w < 0.0f || w > 1.0f)
        return 0;
    
    t = kep::dot(edge2, qvec) * inv_det;// 8 flops
    if(t < 0.0f)
        return 0;
    *o_point = _ray->s + _ray->d * t; // 6 flops
    return 1;;
}

int PathFinder::getLoc(Loc * o_loc, kep::Vector3 _pos) //returns position of where the agent is and the triangle that is in, _pos must be a position within the world above the the nav mesh 
{
    Ray r(kep::Vector3(_pos.x, _pos.y + 1.0f, _pos.z), kep::Vector3(0.0f, -1.0f, 0.0f));
    kep::Vector3 p(0.0f ,0.0f, 0.0f);
    
    TriangleO * t = NULL;
    
    for(uint i = 0; i<m_navMeshSort.size(); i++)
        if(rayTriangleMT97(&r, m_navMeshSort[i], &p) == 1)
        {
            t = m_navMeshSort[i];
            break;
        }
        
    if(t != NULL)
    {
        o_loc->tri = t;
        o_loc->pos = kep::Vector3(p.x, p.y+1.0f, p.z);// shift y pos up by 1
        return 0;
    }
    else //could not get the location, most likely the input position is outside the navigation mesh
    {
        //TODO
        //find the triangle closest to the input pos and return that as the location
        return 1;
    }
    
}


