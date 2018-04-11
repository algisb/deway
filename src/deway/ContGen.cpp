#include "ContGen.h"
#include "Region.h"
#include "Edge.h"
#include "Voxel.h"
#include "AABB.h"
#include "Contour.h"
#include "Vertex.h"
using namespace deway;
ContGen::ContGen(std::vector<Region*> * _regions)
{
    m_regions_ref = _regions;
    genEdges();
    flagExtEdges();
    traceContours();
    extractContourVertex();
    genSegments();
    reduceVerts();
}

ContGen::~ContGen()
{
    for(uint i = 0; i<m_edge.size(); i++)
        delete m_edge[i];
    
    for(uint i = 0; i<m_contours.size(); i++)
        delete m_contours[i];
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
                
                v->up_e->v[0] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                v->up_e->v[1] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                
                //Compare Y values with neighbour voxels and set vertex y values to the largest one
                //V0
                int v0_cl[3] = {0,7,6};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v0_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v0_cl[l]]->aabb->c.y + v->nghbr[v0_cl[l]]->aabb->hs.y);
                        if(nu_y > v->up_e->v[0].y)
                            v->up_e->v[0].y = nu_y;
                    }
                
                int v1_cl[3] = {0,1,2};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v1_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v1_cl[l]]->aabb->c.y + v->nghbr[v1_cl[l]]->aabb->hs.y);
                        if(nu_y > v->up_e->v[1].y)
                            v->up_e->v[1].y = nu_y;
                    }
                
                
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
                
                v->left_e->v[0] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                v->left_e->v[1] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                
                //Compare Y values with neighbour voxels and set vertex y values to the largest one
                //V0
                int v0_cl[3] = {0,1,2};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v0_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v0_cl[l]]->aabb->c.y + v->nghbr[v0_cl[l]]->aabb->hs.y);
                        if(nu_y > v->left_e->v[0].y)
                            v->left_e->v[0].y = nu_y;
                    }
                
                int v1_cl[3] = {2,3,4};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v1_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v1_cl[l]]->aabb->c.y + v->nghbr[v1_cl[l]]->aabb->hs.y);
                        if(nu_y > v->left_e->v[1].y)
                            v->left_e->v[1].y = nu_y;
                    }
                
                
                
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
                
                v->down_e->v[0] = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                v->down_e->v[1] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                
                //Compare Y values with neighbour voxels and set vertex y values to the largest one
                //V0
                int v0_cl[3] = {2,3,4};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v0_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v0_cl[l]]->aabb->c.y + v->nghbr[v0_cl[l]]->aabb->hs.y);
                        if(nu_y > v->down_e->v[0].y)
                            v->down_e->v[0].y = nu_y;
                    }
                
                int v1_cl[3] = {4,5,6};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v1_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v1_cl[l]]->aabb->c.y + v->nghbr[v1_cl[l]]->aabb->hs.y);
                        if(nu_y > v->down_e->v[1].y)
                            v->down_e->v[1].y = nu_y;
                    }
                
                
                
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
                
                v->right_e->v[0] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                v->right_e->v[1] = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                
                //Compare Y values with neighbour voxels and set vertex y values to the largest one
                //V0
                int v0_cl[3] = {4,5,6};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v0_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v0_cl[l]]->aabb->c.y + v->nghbr[v0_cl[l]]->aabb->hs.y);
                        if(nu_y > v->right_e->v[0].y)
                            v->right_e->v[0].y = nu_y;
                    }
                
                int v1_cl[3] = {0,7,6};
                for(uint l = 0; l<3; l++)
                    if(v->nghbr[v1_cl[l]] != NULL)
                    {
                        float nu_y = (v->nghbr[v1_cl[l]]->aabb->c.y + v->nghbr[v1_cl[l]]->aabb->hs.y);
                        if(nu_y > v->right_e->v[1].y)
                            v->right_e->v[1].y = nu_y;
                    }
                
                
                
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

void ContGen::traceContours()
{
    for(uint i = 0; i<m_regions_ref->size(); i++)
    {
        deway::Region & rr = (*(*m_regions_ref)[i]);
        Contour * con = new Contour();
        
        Edge * initEdge = NULL;
        Voxel * initVoxel = NULL;
        int direction = 0;
        
        //find the first external edge
        for(uint j = 0; j< rr.size(); j++)
        {
            for(uint k = 0; k<4; k++)
            {
                if(rr.m_voxels[j]->edg[k]->external)
                {
                    initEdge = rr.m_voxels[j]->edg[k];
                    initVoxel = rr.m_voxels[j];
                }
                
                if(initEdge != NULL)
                    break;
            }
            if(initEdge != NULL)
                    break;
        }
        
        //find the initial direction of the tracer
        for(uint j = 0; j<4; j++)
        {
            if(initVoxel->edg[j] == initEdge)
            {
                direction = j;
                break;
            }
        }
        //printf("dir: %d \n", direction);
        con->m_cntrE.push_back(initEdge);//add initial edge to the contour
        
        
        
        
        //START of the tracing algorithm////////////////////////////////////////////
        Edge * currentEdge = initEdge;
        Voxel * currentVoxel = initVoxel;
        bool cw = true; // clockwise
        
        while(1)
        {
            //rotate the tracer
            if(cw)
            {
                direction++;
                if(direction > 3)
                    direction = 0;
            }
            else
            {
                direction--;
                if(direction < 0)
                    direction = 3;
            }
            
            currentEdge = currentVoxel->edg[direction];
            if(currentEdge->external)
            {
                cw = true;//set next rotation dir
                if(currentEdge == initEdge)//check if the tracing is complete
                    break;
                else
                    con->m_cntrE.push_back(currentEdge);//add edge to the contour
            }
            else
            {
                cw = false;//set next rotation dir
                //move on to the neighbour voxel
                if(currentEdge->nghbr[0] != currentVoxel)//should not need to check for NULL neighbour as these are internal edges
                    currentVoxel = currentEdge->nghbr[0];
                else
                    currentVoxel = currentEdge->nghbr[1];
            }
        }
        /////////////////////////////////////////////////////////////////////////////
        //printf("num edg: %d \n", con->m_contour.size());
        
        m_contours.push_back(con);//add the contour to the list of contours
    }

}

void ContGen::extractContourVertex()
{
    for(uint i = 0; i<m_contours.size(); i++)
    {
        Edge * prevEdge = m_contours[i]->m_cntrE[m_contours[i]->m_cntrE.size()-1];
        for(uint j = 0; j<m_contours[i]->m_cntrE.size(); j++)
        {
            Edge * tmpEdge = m_contours[i]->m_cntrE[j];
            Vertex * vert = new Vertex();
            
            vert->pos = tmpEdge->v[0];
            
            
            //find NULL regions
            void * currentRegs[2] = {NULL,NULL};
            void * prevRegs[2] = {NULL,NULL};
            
            for(uint k = 0; k<2; k++)
            {
                if(tmpEdge->nghbr[k] != NULL)
                    currentRegs[k] = tmpEdge->nghbr[k]->reg;
                
                if(prevEdge->nghbr[k] != NULL)
                    prevRegs[k] = prevEdge->nghbr[k]->reg;
            }
            
            if(//Check if this vertex needs to be locked
                !((prevRegs[0] == currentRegs[0]  &&  prevRegs[1] == currentRegs[1]) ||
                (prevRegs[0] == currentRegs[1]  &&  prevRegs[1] == currentRegs[0]))
            )
            {
                vert->locked = true;
            }
            
            m_contours[i]->m_verts.push_back(vert);
            prevEdge = tmpEdge;
        }
    }
    
    //add 2 locked verticies for regions with 0 region connections
    //TODO
    
    //printf("num edg: %d \n", m_contours[0]->m_cntrE.size());
    //printf("numvert: %d \n", m_contours[0]->m_cntrV.size());
}


void ContGen::genSegments()//Prepares the contours for verticies for reduction algorithm
{
    
    for(uint i = 0; i<m_contours.size(); i++)
    {
        ////////////////////////////////////////////////////////
        Vertex * initVertex = NULL;
        bool end = false;
        for(uint j = 0; j<m_contours[i]->m_verts.size(); j++)//for each vertex in the contour
        {
            if(m_contours[i]->m_verts[j]->locked)//found the starting point
            {
                std::vector<Vertex*> segment;
                
                if(initVertex == NULL)
                    initVertex = m_contours[i]->m_verts[j];
                
                segment.push_back(m_contours[i]->m_verts[j]);
                
                uint iter = j+1;

                
                while(1)//find the end point
                {
                    if(iter > (m_contours[i]->m_verts.size()-1))//warp to the begining
                        iter = 0;
                    
                    segment.push_back(m_contours[i]->m_verts[iter]);
                    if(m_contours[i]->m_verts[iter]->locked)
                    {
                        if(m_contours[i]->m_verts[iter] == initVertex)
                            end = true;
                        break;
                    }
                    
                    iter++;
                }
                
                m_contours[i]->m_segments.push_back(segment);
                //printf("segment: %d \n", segment.size());
            }
            if(end)
                break;
            
        }
        ////////////////////////////////////////////////////////
    }
    
}

void ContGen::reduceVerts() //Douglas peucker algorithm
{
    //CONFIG PARAMS
    float tollerance = 0.5f;
    //////////////////
    
    for(uint i = 0; i<m_contours.size(); i++)
    {
        printf("Cont %d \n -----------", i);
        printf("Num segments: %d \n", m_contours[i]->m_segments.size());
        //printf("segment: %d \n", segment.size());
    }
}


