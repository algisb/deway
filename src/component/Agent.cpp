#include "Agent.h"
#include "deway/pathfinder/PathFinder.h"
#include "deway/Vertex.h"
#include "deway/pathfinder/Loc.h"
#include "deway/TriangleO.h"
using namespace kelp;

Agent::Agent(deway::PathFinder * _pathFinder) : Component()
{
    m_pathFinder = _pathFinder;
}
Agent::~Agent()
{
}

void Agent::init()
{
    m_transform = m_owner->getComponent<Transform>();
}

void Agent::update()
{
    
}

void Agent::render()
{
    
}

void Agent::tracePath(deway::Vertex * _endVertex, std::vector<kep::Vector3> * o_path)
{
    std::vector<kep::Vector3> revPath;
    deway::Vertex * currentVertex = _endVertex;
    
    while(currentVertex != NULL)
    {
        revPath.push_back(kep::Vector3( currentVertex->pos.x, currentVertex->pos.y + 1.0f, currentVertex->pos.z));
        currentVertex = currentVertex->parent;
    }
    //printf("rev list size: %d \n", revPath.size());
    for(int i = revPath.size() - 1; i > -1; i--)
    {
        o_path->push_back(revPath[i]);
    }
}
bool Agent::findVertex(deway::Vertex* _vertToFind, std::vector<deway::Vertex *>* _container)
{
    for(uint i = 0; i < _container->size(); i++)
        if((*_container)[i] == _vertToFind)
            return true;
        
    return false;
}


int Agent::genPath(deway::Loc * _finish, std::vector<kep::Vector3> * o_path)
{
    deway::Loc start;
    m_pathFinder->getLoc(&start, m_transform->m_position);
    
    //start and finish location is in the same triangle
    if(start.tri == _finish->tri)
    {
        o_path->push_back(start.pos);
        o_path->push_back(_finish->pos);
        return 0;
    }
    else//Run A*
    {
        std::vector<deway::Vertex*> openList;
        std::vector<deway::Vertex*> closedList;
        
        
        deway::Vertex * startVerts[3] = {start.tri->vertex[0], start.tri->vertex[1], start.tri->vertex[2]};
        deway::Vertex * finishVerts[3] = {_finish->tri->vertex[0], _finish->tri->vertex[1], _finish->tri->vertex[2]};
        
        
        for(uint i = 0; i<3; i++)
        {
            startVerts[i]->G_cost = 0.0f;
            startVerts[i]->H_cost = (_finish->pos - startVerts[i]->pos).magnitude();
            startVerts[i]->F_cost = startVerts[i]->G_cost + startVerts[i]->H_cost;
            openList.push_back(startVerts[i]);
        }
        
        while(openList.size() > 0)
        {
            deway::Vertex * currentVert = NULL;
            uint index = 0;
            //find the cheapest vertex
            for(uint i = 0; i<openList.size(); i++)
            {
                if(currentVert != NULL)
                {
                    if(openList[i]->F_cost < currentVert->F_cost)
                    {
                        currentVert = openList[i];
                        index = i;
                    }
                }
                else
                {
                    currentVert = openList[i];
                    index = i;
                }
            }
            
            //delete current vertex from open list and add it to closed list
            closedList.push_back(openList[index]);
            openList.erase(openList.begin() + index);
            
            //check if we GATEEEM
            for(uint i = 0; i<3; i++)
            {
                if(currentVert == finishVerts[i])
                {
                    //trace the path
                    tracePath(currentVert, o_path);
                    //TODO:Add the detailed positions to the path
                    //printf("got path %d \n", o_path->size());
                    return 0;
                }
            }
            
            //loop of doom
            for(uint i = 0; i < currentVert->nghbr.size(); i++)
                for(uint j = 0; j < 3; j++)
                {
                    deway::Vertex * n = currentVert->nghbr[i]->vertex[j];
                    if(!findVertex(n, &closedList))
                    {
                        bool inOpen = findVertex(n, &openList);
                        float tG_cost = currentVert->G_cost + (currentVert->pos - n->pos).magnitude();
                        if(!inOpen || n->G_cost > tG_cost)
                        {
                            n->G_cost = tG_cost;
                            n->H_cost = (_finish->pos - n->pos).magnitude();
                            n->F_cost = n->G_cost + n->H_cost;
                            
                            n->parent = currentVert;
                            if(!inOpen)
                                openList.push_back(n);
                        }
                    }
                    
                }
                
            
            
            
            
            
        }
    }//////////////////////////////////////////////////////////////
    
    printf("Cant find a valid path\n");
    return 1;
}




