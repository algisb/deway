#include "TriGen.h"
#include "TriangleO.h"
#include "Contour.h"
#include "Vertex.h"
using namespace deway;
TriGen::TriGen(std::vector<Contour*> * _contours)
{
    m_contours_ref = _contours;
    genTriangles();
}
TriGen::~TriGen()
{
    for(uint i = 0; i<m_navMesh.size(); i++)
        delete m_navMesh[i];
}
void TriGen::genTriangles()
{
    //4 -5
    for(uint i = 0; i<m_contours_ref->size(); i++) //for each contour
    {    
        std::vector<Vertex*> verts;
        for(uint j = 0; j<(*m_contours_ref)[i]->m_reducedVerts.size(); j++)//create another vertex list
            verts.push_back((*m_contours_ref)[i]->m_reducedVerts[j]);
        
        while(verts.size() > 2) //continue triangulating 
        {
            Vertex * v[3] = {NULL, NULL, NULL};//actual verticies
            float dist = 0;
            uint index = 0;
            
            for(uint k = 0; k < verts.size(); k++)
            {
                uint i0 = k;
                uint i1 = k+1;
                uint i2 = k+2;
                if(i2 > verts.size()-1)
                    break;//i2 = i2 - (verts.size()-1);
                if(i1 > verts.size()-1)
                    break;//i1 = i1 -(verts.size()-1);
                
                
                Vertex * pv[3] = {verts[i0], verts[i1], verts[i2]};//potencial verticies
                float pdist =  (kep::Vector2(pv[2]->pos.x, pv[2]->pos.z) - kep::Vector2(pv[0]->pos.x, pv[0]->pos.z)).magnitude(); //(pv[2]->pos - pv[0]->pos).magnitude();
                // Assume we are at the n-th index.
                kep::Vector2 prev(pv[0]->pos.x, pv[0]->pos.z);
                
                kep::Vector2 curr(pv[1]->pos.x, pv[1]->pos.z); 
                
                kep::Vector2 next(pv[2]->pos.x, pv[2]->pos.z);
                

                kep::Vector2 one = prev - curr;
                kep::Vector2 two = next - curr;
                kep::Vector2 n(one.y, -one.x);

                float magnitude = n.magnitude() * two.magnitude();
                float angl = (acos(kep::dot(n,two)) / magnitude);
                angl *= 180.0f / 3.14159265359f;    // Convert to degrees.

                if (!((curr.x - prev.x) * (next.y - curr.y) - (next.x - curr.x) * (curr.y - prev.y) > 0))
                    if(pdist < dist || (v[0] == NULL))
                    {
                        v[0] = pv[0];
                        v[1] = pv[1];
                        v[2] = pv[2];
                        dist = pdist;
                        index = i1;
                    }
                
            }
            if(v[0] != NULL)
                m_navMesh.push_back(new TriangleO(v[0], v[1], v[2]));// add the triangle to the nav mesh
                
            verts.erase(verts.begin() + index);
            
        }
    }
}

