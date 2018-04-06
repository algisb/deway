#include "ContGen.h"
#include "Region.h"
#include "Edge.h"
#include "Voxel.h"
#include "AABB.h"
#include "Contour.h"
using namespace deway;
ContGen::ContGen(std::vector<Region*> * _regions)
{
    m_regions_ref = _regions;
    genEdges();
    flagExtEdges();
    genContours();
}

ContGen::~ContGen()
{
    for(uint i = 0; i<m_edge.size(); i++)
        delete m_edge[i];
}

void ContGen::genEdges()
{
    for(uint i = 0; i < m_regions_ref->size(); i++)
    {
        Region * r = (*m_regions_ref)[i];
        for(uint j = 0; j<r->m_voxels.size(); j++)
        {
            Voxel * v = r->m_voxels[j];
            
            //UP EDGE
            if(v->up != NULL)
            {
                 if(v->up->down_e != NULL)
                 {
                     v->up_e = v->up->down_e;
                     v->up_e->nghbr[1] = v;
                 }
            }
            if(v->up_e == NULL)
            {
                v->up_e = new Edge();
                v->up_e->nghbr[0] = v;
                
                v->up_e->v[0] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                v->up_e->v[1] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                
                
                m_edge.push_back(v->up_e);
            }
            /////////////////////////////////////////////////////////////////////
            
             //LEFT EDGE
            if(v->left != NULL)
            {
                if(v->left->right_e != NULL)
                {
                    v->left_e = v->left->right_e;
                    v->left_e->nghbr[1] = v;
                }
            }
            if(v->left_e == NULL)
            {
                v->left_e = new Edge();
                v->left_e->nghbr[0] = v;
                
                v->left_e->v[0] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                v->left_e->v[1] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                m_edge.push_back(v->left_e);
            }
            /////////////////////////////////////////////////////////////////////
            
            //DOWN EDGE
            if(v->down != NULL)
            {
                if(v->down->up_e != NULL)
                {
                    v->down_e = v->down->up_e;
                    v->down_e->nghbr[1] = v;
                }
            }
            if(v->down_e == NULL)
            {
                v->down_e = new Edge();
                v->down_e->nghbr[0] = v;
                
                v->down_e->v[0] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                v->down_e->v[1] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                m_edge.push_back(v->down_e);
            }
            /////////////////////////////////////////////////////////////////////
            
            //RIGHT EDGE
            if(v->right != NULL)
            {
                if(v->right->left_e != NULL)
                {
                    v->right_e = v->right->left_e;
                    v->right_e->nghbr[1] = v;
                }
            }
            if(v->right_e == NULL)
            {
                v->right_e = new Edge();
                v->right_e->nghbr[0] = v;
                
                v->right_e->v[0] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                v->right_e->v[1] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                m_edge.push_back(v->right_e);
            }
            /////////////////////////////////////////////////////////////////////
            
        }
    }
    

}

void ContGen::flagExtEdges()
{
    //flag external regions
    for(uint i = 0; i < m_edge.size(); i++)
    {
        if(m_edge[i]->nghbr[0] == NULL || m_edge[i]->nghbr[1] == NULL)
            m_edge[i]->external = true;
         else
             if(m_edge[i]->nghbr[0]->reg != m_edge[i]->nghbr[1]->reg)
                 m_edge[i]->external = true;
    }
}

void ContGen::genContours()
{
    for(uint i = 0; i<m_regions_ref->size(); i++)
    {
        deway::Region & rr = (*(*m_regions_ref)[i]);
        Contour * con = new Contour();
        Edge * initEdge = NULL;
        
        
        //find the first external edge
        for(uint j = 0; j< rr.size(); j++)
        {
            for(uint k = 0; k<4; k++)
            {
                if(rr.m_voxels[j]->edg[k]->external)
                    initEdge = rr.m_voxels[j]->edg[k];
                
                if(initEdge != NULL)
                    break;
            }
            if(initEdge != NULL)
                    break;
        }
        
        
        
        
        
    }

}


