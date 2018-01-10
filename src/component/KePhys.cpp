///
///  @file KePhys.cpp
///  @brief links between Kep physics engine to kelp render engine 

#include "KePhys.h"
#include "Transform.h"
using namespace kelp;
KePhys::KePhys(kep::RigidBody * _rigidBody)
{
    m_rigidBody = _rigidBody;
}
KePhys::~KePhys()
{
}
void KePhys::init()
{
    //externally defined values can only be hooked after initilization
    //m_rigidBody->position = &m_owner->m_transform->m_position;
    //m_rigidBody->orientation = &m_owner->m_transform->m_orientation;
    //m_owner->m_world->m_physWorld->addRigidBody(m_rigidBody);
    //m_rigidBody->calculateDerivedData();
}
void KePhys::update()
{
}
void KePhys::render()
{
}



