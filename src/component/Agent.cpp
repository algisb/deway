#include "Agent.h"
#include "deway/pathfinder/PathFinder.h"
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



