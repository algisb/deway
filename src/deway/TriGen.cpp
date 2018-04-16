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

void TriGen::earClip()
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
            
            //first pass all the with all required checks for EAR CLIPPING triangulation algorithm
            for(uint k = 0; k < verts.size(); k++)
            {
                uint i0 = k;
                uint i1 = k+1;
                uint i2 = k+2;
                if(i2 > (verts.size()-1))
                    /*break;*/i2 = i2 - (verts.size());
                if(i1 > (verts.size()-1))
                    /*break;*/i1 = i1 -(verts.size());
                
                Vertex * pv[3] = {verts[i0], verts[i1], verts[i2]};//potencial verticies
                kep::Vector2 prev(pv[0]->pos.x, pv[0]->pos.z);
                kep::Vector2 curr(pv[1]->pos.x, pv[1]->pos.z); 
                kep::Vector2 next(pv[2]->pos.x, pv[2]->pos.z);
                float pdist =  (next - prev).magnitude();
                
                bool validTri = true;
                float denom = (curr.y - next.y) * (prev.x - next.x) + 
                                (next.x - curr.x) * (prev.y - next.y);
                                
                // Avoid division by zero.
                if(denom == 0.0f) continue;
                denom = 1.0f / denom;
                          
                
                for(uint l = 0; l<verts.size(); l++)//check if all other vetrices are outside our new triangle
                {
                    if(verts[l] == pv[0] || verts[l] == pv[1] || verts[l] == pv[2])
                        continue;
                    
                    
                    kep::Vector2 p(verts[l]->pos.x, verts[l]->pos.z);

                    float alpha = denom * ((curr.y - next.y) * (p.x - next.x) + 
                                        (next.x - curr.x) * (p.y - next.y));

                    float beta  = denom * ((next.y - prev.y) * (p.x - next.x) + 
                                        (prev.x - next.x) * (p.y - next.y));

                    float gamma = 1.0f - alpha - beta;

                    if(!(alpha < 0.0f || beta  < 0.0f || gamma < 0.0f))//the point is NOT in the triangle.
                    {
                        validTri = false;
                        break;
                    }
                }
                
                // Three points are a counter-clockwise turn if ccw > 0, clockwise if
                // ccw < 0, and collinear if ccw = 0 because ccw is a determinant that
                // gives twice the signed  area of the triangle formed by p1, p2 and p3.
                if(validTri)
                    if (!((curr.x - prev.x) * (next.y - curr.y) - (next.x - curr.x) * (curr.y - prev.y) > 0)) //use the determinant to check if the hull is convex
                        if(pdist < dist || (v[0] == NULL)) //create triangles out of partition with smalles segment
                        {
                            v[0] = pv[0];
                            v[1] = pv[1];
                            v[2] = pv[2];
                            dist = pdist;
                            index = i1;
                        }
                
            }
            
            //Second pass without checking if other points are inside the triangle
            if(v[0] == NULL)
                for(uint k = 0; k < verts.size(); k++)
                {
                    uint i0 = k;
                    uint i1 = k+1;
                    uint i2 = k+2;
                    if(i2 > (verts.size()-1))
                        i2 = i2 - (verts.size());
                    if(i1 > (verts.size()-1))
                        i1 = i1 -(verts.size());
                    
                    Vertex * pv[3] = {verts[i0], verts[i1], verts[i2]};//potencial verticies
                    kep::Vector2 prev(pv[0]->pos.x, pv[0]->pos.z);
                    kep::Vector2 curr(pv[1]->pos.x, pv[1]->pos.z); 
                    kep::Vector2 next(pv[2]->pos.x, pv[2]->pos.z);
                    float pdist =  (next - prev).magnitude(); //(pv[2]->pos - pv[0]->pos).magnitude();
                    
                    if (!((curr.x - prev.x) * (next.y - curr.y) - (next.x - curr.x) * (curr.y - prev.y) > 0)) //use the determinant to check if the hull is convex
                        if(pdist < dist || (v[0] == NULL)) //create triangles out of partition with smalles segment
                        {
                            v[0] = pv[0];
                            v[1] = pv[1];
                            v[2] = pv[2];
                            dist = pdist;
                            index = i1;
                        }
                }
    
    
            if(v[0] != NULL)//this should never happen, but since our programmed algorithm sucks it might happen - SOLUTION : use mesh triangulation third party library
                m_navMesh.push_back(new TriangleO(v[0], v[1], v[2]));// add the triangle to the nav mesh
            else
            {
                printf("ERROR line:%d file:%s - TriGen skipping a triangle due to unmet conditions, broken navmesh \n", __LINE__, __FILE__);
            }

            verts.erase(verts.begin() + index);
            
        }
    }
}

void TriGen::genTriangles()
{
    earClip();
}

