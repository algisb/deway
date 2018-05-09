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
    
    for(uint i = 0; i<m_vertex.size(); i++)
        delete m_vertex[i];
}

void ContGen::genEdges()
{
    for(uint i = 0; i < m_regions_ref->size(); i++)
    {
        Region * r = (*m_regions_ref)[i];
        for(uint j = 0; j<r->m_voxels.size(); j++)
        {
            Voxel * v = r->m_voxels[j];
            Vertex * topLeft = NULL;
            Vertex * topRight = NULL;
            Vertex * botLeft = NULL;
            Vertex * botRight = NULL;
            
            //UP EDGE
            if(v->up != NULL)
            {
                 if(v->up->down_e != NULL)
                 {
                     v->up_e = v->up->down_e;
                     v->up_e->nghbr[1] = v;
                     if(topLeft == NULL)
                        topLeft = v->up->down_e->vertex[1];
                     if(topRight == NULL)
                        topRight = v->up->down_e->vertex[0];
                 }
            }
            
            //DOWN EDGE
            if(v->down != NULL)
            {
                if(v->down->up_e != NULL)
                {
                    v->down_e = v->down->up_e;
                    v->down_e->nghbr[1] = v;
                    if(botLeft == NULL)
                        botLeft = v->down->up_e->vertex[0];
                    if(botRight == NULL)
                        botRight = v->down->up_e->vertex[1];
                }
            }
            
            //LEFT EDGE
            if(v->left != NULL)
            {
                if(v->left->right_e != NULL)
                {
                    v->left_e = v->left->right_e;
                    v->left_e->nghbr[1] = v;
                    
                    if(topLeft == NULL)
                        topLeft = v->left->right_e->vertex[0];
                    if(botLeft == NULL)
                        botLeft = v->left->right_e->vertex[1];
                }
            }
            
            //RIGHT EDGE
            if(v->right != NULL)
            {
                if(v->right->left_e != NULL)
                {
                    v->right_e = v->right->left_e;
                    v->right_e->nghbr[1] = v;
                    
                    if(topRight == NULL)
                        topRight = v->right->left_e->vertex[1];
                    if(botRight == NULL)
                        botRight = v->right->left_e->vertex[0];
                }
            }
            
            //Find verticies-----------------------------------------------------------------------------------
            if(topLeft == NULL)
            {
                //find topLeft///////////////////////////////////
                //upper neighbour
                if(v->up != NULL)
                {
                    if(v->up->down_e != NULL)
                        topLeft = v->up->down_e->vertex[1];
                    if(v->up->left_e != NULL)
                        topLeft = v->up->left_e->vertex[0];
                }
                //top left neighbour
                if(v->up_left != NULL)
                {
                    if(v->up_left->down_e != NULL)
                        topLeft = v->up_left->down_e->vertex[0];
                    if(v->up_left->right_e != NULL)
                        topLeft = v->up_left->right_e->vertex[1];
                }
                //left neighbour
                if(v->left != NULL)
                {
                    if(v->left->up_e != NULL)
                        topLeft = v->left->up_e->vertex[1];
                    if(v->left->right_e != NULL)
                        topLeft = v->left->right_e->vertex[0];
                }
                
                //Create a new vertex as it does not exist
                if(topLeft == NULL)
                {
                    topLeft = new Vertex();
                    topLeft->pos = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                    
                    //Compare Y values with neighbour voxels and set vertex y values to the largest one
                    int v0_cl[3] = {0,1,2};
                    for(uint l = 0; l<3; l++)
                        if(v->nghbr[v0_cl[l]] != NULL)
                        {
                            float nu_y = (v->nghbr[v0_cl[l]]->aabb->c.y + v->nghbr[v0_cl[l]]->aabb->hs.y);
                            if(nu_y > topLeft->pos.y)
                                topLeft->pos.y = nu_y;
                        }
                    m_vertex.push_back(topLeft);
                }
            }
            
            if(topRight == NULL)
            {
                //find topRight///////////////////////////////////
                //upper neighbour
                if(v->up != NULL)
                {
                    if(v->up->down_e != NULL)
                        topRight = v->up->down_e->vertex[0];
                    if(v->up->right_e != NULL)
                        topRight = v->up->right_e->vertex[1];
                }
                //top right neighbour
                if(v->up_right != NULL)
                {
                    if(v->up_right->left_e != NULL)
                        topRight = v->up_right->left_e->vertex[0];
                    if(v->up_right->down_e != NULL)
                        topRight = v->up_right->down_e->vertex[1];
                }
                //right neighbour
                if(v->right != NULL)
                {
                    if(v->right->up_e != NULL)
                        topRight = v->right->up_e->vertex[0];
                    if(v->right->left_e != NULL)
                        topRight = v->right->left_e->vertex[1];
                }
                if(topRight == NULL)
                {
                    topRight = new Vertex();
                    topRight->pos = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, v->aabb->hs.z);
                    
                    int v1_cl[3] = {0,7,6};
                    for(uint l = 0; l<3; l++)
                        if(v->nghbr[v1_cl[l]] != NULL)
                        {
                            float nu_y = (v->nghbr[v1_cl[l]]->aabb->c.y + v->nghbr[v1_cl[l]]->aabb->hs.y);
                            if(nu_y > topRight->pos.y)
                                topRight->pos.y = nu_y;
                        }
                    m_vertex.push_back(topRight);
                }
            }
            
            if(botLeft == NULL)
            {
                //find botLeft///////////////////////////////////
                if(v->down != NULL)
                {
                    if(v->down->up_e != NULL)
                        botLeft = v->down->up_e->vertex[0];
                    if(v->down->left_e != NULL)
                        botLeft = v->down->left_e->vertex[1];
                }
                if(v->down_left != NULL)
                {
                    if(v->down_left->up_e != NULL)
                        botLeft = v->down_left->up_e->vertex[1];
                    if(v->down_left->right_e != NULL)
                        botLeft = v->down_left->right_e->vertex[0];
                }
                if(v->left != NULL)
                {
                    if(v->left->down_e != NULL)
                        botLeft = v->left->down_e->vertex[0];
                    if(v->left->right_e != NULL)
                        botLeft = v->left->right_e->vertex[1];
                }
                if(botLeft == NULL)
                {
                    botLeft = new Vertex();
                    botLeft->pos = v->aabb->c + kep::Vector3(v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                    
                    //Compare Y values with neighbour voxels and set vertex y values to the largest one
                    int v0_cl[3] = {2,3,4};
                    for(uint l = 0; l<3; l++)
                        if(v->nghbr[v0_cl[l]] != NULL)
                        {
                            float nu_y = (v->nghbr[v0_cl[l]]->aabb->c.y + v->nghbr[v0_cl[l]]->aabb->hs.y);
                            if(nu_y > botLeft->pos.y)
                                botLeft->pos.y = nu_y;
                        }
                        m_vertex.push_back(botLeft);
                }
            }
            
            if(botRight == NULL)
            {
                //find botRight///////////////////////////////////
                if(v->down != NULL)
                {
                    if(v->down->up_e != NULL)
                        botRight = v->down->up_e->vertex[1];
                    if(v->down->right_e != NULL)
                        botRight = v->down->right_e->vertex[0];
                }
                if(v->down_right != NULL)
                {
                    if(v->down_right->up_e != NULL)
                        botRight = v->down_right->up_e->vertex[0];
                    if(v->down_right->left_e != NULL)
                        botRight = v->down_right->left_e->vertex[1];
                }
                if(v->right != NULL)
                {
                    if(v->right->left_e != NULL)
                        botRight = v->right->left_e->vertex[0];
                    if(v->right->down_e != NULL)
                        botRight = v->right->down_e->vertex[1];
                }
                if(botRight == NULL)
                {
                    botRight = new Vertex();
                    botRight->pos = v->aabb->c + kep::Vector3(-v->aabb->hs.x, v->aabb->hs.y, -v->aabb->hs.z);
                    
                    int v1_cl[3] = {4,5,6};
                    for(uint l = 0; l<3; l++)
                        if(v->nghbr[v1_cl[l]] != NULL)
                        {
                            float nu_y = (v->nghbr[v1_cl[l]]->aabb->c.y + v->nghbr[v1_cl[l]]->aabb->hs.y);
                            if(nu_y > botRight->pos.y)
                                botRight->pos.y = nu_y;
                        }
                        m_vertex.push_back(botRight);
                    
                }
            }
            /////////////////////////////////////////////////////////////////////
            
            if(v->up_e == NULL)
            {                
                Edge * edge = new Edge();
                edge->vertex[0] = topLeft;
                edge->vertex[1] = topRight;
                
                edge->nghbr[0] = v;
                
                m_edge.push_back(edge);
                v->up_e = edge;
            }
            
            if(v->down_e == NULL)
            {
                
                Edge * edge = new Edge();
                edge->vertex[0] = botRight;
                edge->vertex[1] = botLeft;
                
                edge->nghbr[0] = v;
                
                m_edge.push_back(edge);
                v->down_e = edge;
                
            }
            
            /////////////////////////////////////////////////////////////////////
            

            if(v->left_e == NULL)
            {
                Edge * edge = new Edge();
                edge->vertex[0] = botLeft;
                edge->vertex[1] = topLeft;
                
                edge->nghbr[0] = v;
                
                m_edge.push_back(edge);
                v->left_e = edge;
            }
            

            if(v->right_e == NULL)
            {
                Edge * edge = new Edge();
                edge->vertex[0] = topRight;
                edge->vertex[1] = botRight;
                
                edge->nghbr[0] = v;
                
                m_edge.push_back(edge);
                v->right_e = edge;
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
        con->reg = (*m_regions_ref)[i];
        
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
            Vertex * vert = NULL;
            //vert = tmpEdge->vertex[0];
            ///////////////////////////////////////////////////////////////////////Cheeky little bug
            //find which edge verts to keep
            if(tmpEdge->nghbr[1] == NULL)
            {
                vert = tmpEdge->vertex[1];
            }
            else
            {
                    if(tmpEdge->nghbr[0]->reg == m_contours[i]->reg)//which voxel does the edge get traced from
                    {
                            vert = tmpEdge->vertex[1];
                    }
                    else if(tmpEdge->nghbr[1]->reg == m_contours[i]->reg)//which voxel does the edge get traced from
                    {

                            vert = tmpEdge->vertex[0];
                    }
            }
            /////////////////////////////////////////////////////////////////////////////////////////
           
            //find NULL regions
            Region* currentRegs[2] = {NULL,NULL};
            Region * prevRegs[2] = {NULL,NULL};
            
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
    
    //lock 2 verticies for regions with 0 region connections
    for(uint i = 0; i<m_contours.size(); i++)
    {
        uint locks = false;
        for(uint j = 0; j<m_contours[i]->m_verts.size(); j++)
            if(m_contours[i]->m_verts[j]->locked)
            {
                locks = true;
                break;
            }
            
        if(!locks && m_contours[i]->m_verts.size() > 2)
        {
            Vertex * v[2] = {m_contours[i]->m_verts[0], m_contours[i]->m_verts[1]};
            float d = (v[1]->pos - v[0]->pos).magnitude();
            
            for(uint j = 2; j<m_contours[i]->m_verts.size(); j++)
            {
                float dn = (m_contours[i]->m_verts[j]->pos - v[0]->pos).magnitude();
                if(dn > d)
                {
                    v[1] = m_contours[i]->m_verts[j];
                    d = dn;
                }
            }
            v[0]->locked = true; //lock the first vertex
            v[1]->locked = true; //lock the second vertex
        }
            
        
    }
    
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

void ContGen::RDP_simp(std::vector<Vertex*> * _segment, float _tollerance, uint _start, uint _end) //Ramer–Douglas–Peucker algorithm adaptation (Recursive function)
{
    if(_segment->size() < 3)
        return;
    
    kep::Vector3 d = ((*_segment)[_end]->pos - (*_segment)[_start]->pos).normalized(); //edge from start point to finish
    //d.normalize();
    
    Vertex * nv = NULL;
    float nvDisp = 0.0f;
    uint nvIndex = 0;
    
    for(uint i = _start+1; i<= _end-1; i++)//find the vertex with the largest displacement from the edge
    {
        kep::Vector3 vec0 = (*_segment)[i]->pos - (*_segment)[_start]->pos;
        float mag = kep::dot(vec0,d);
        kep::Vector3 p = (d * mag) + (*_segment)[_start]->pos;
        kep::Vector3 vec1 = (*_segment)[i]->pos - p;
        float disp = vec1.magnitude();
        
        if(disp > nvDisp)
        {
            nv = (*_segment)[i];
            nvDisp = disp;
            nvIndex = i;
        }
    }
    if(nvDisp > _tollerance)
        if(nv != NULL) //THIS IS THE ONE
        {
            nv->locked = true;// lock it
            RDP_simp(_segment, _tollerance, _start, nvIndex);
            RDP_simp(_segment, _tollerance, nvIndex, _end);
        }
}


void ContGen::reduceVerts() 
{
    //CONFIG PARAMS
    float tollerance = 1.0f;
    //////////////////
    
    //Apply vertex reduction algorithm to each segment
    for(uint i = 0; i<m_contours.size(); i++)//each contour
        for(uint j = 0; j<m_contours[i]->m_segments.size(); j++)//each segment of the contour
        {
            //Vertex * start = m_contours[i]->m_segments[j][0];
            //Vertex * end = m_contours[i]->m_segments[j][m_contours[i]->m_segments[j].size()-1];
            
            RDP_simp(&m_contours[i]->m_segments[j], tollerance, 0, m_contours[i]->m_segments[j].size()-1);
            //for(uint k = 0; k<)
        }
        
    //Extract locked verticies
    for(uint i = 0; i<m_contours.size(); i++)
        for(uint j = 0; j<m_contours[i]->m_verts.size(); j++)
        {
            if(m_contours[i]->m_verts[j]->locked)
            {
                m_contours[i]->m_reducedVerts.push_back(m_contours[i]->m_verts[j]);
            }
        }
}


