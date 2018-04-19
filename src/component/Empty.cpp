#include "Empty.h"
#include "world/World_0.h"
#include "Input.h"
#include "Time.h"
#include <Core.h>
#include "deway/pathfinder/PathFinder.h"
#include "deway/pathfinder/Loc.h"
#include "deway/TriangleO.h"
#include "deway/Vertex.h"

using namespace kelp;
Empty::Empty(Entity * _playerCamera, KePhys * _agentBody, Agent * _agent, MeshGen * _trigen)
{
    kekX = -8.0f;
    m_playerCameraEntity = _playerCamera;
    m_agentBody_ref = _agentBody;
    m_agent_ref = _agent;
    m_triGen_ref = _trigen;
}
Empty::~Empty()
{
}

void Empty::init()
{
    m_playerCameraComp = m_playerCameraEntity->getComponent<Camera>();
    //m_playerCameraComp->m_possessed = false;//no longer use internal controls
    //Input::Mouse::setMouseJoystickMode();
    
}

void Empty::update()
{
//         const float rotSpd = 70.0f * Time::s_deltaT;
//         const float mouseRotSpd = rotSpd *0.1f;
//         //pitch///////////////////////////////////////
//         float tmpAng = m_playerCameraComp->m_pitchAngle;
//         tmpAng += -mouseRotSpd * Input::Mouse::dy;
//         
//         if(tmpAng < 90.0f && tmpAng > -90.0f)//pitch clamp
//         {
//             m_playerCameraComp->m_pitchAngle = tmpAng;
//             kep::Quaternion q;
//             q.setEuler(m_playerCameraComp->m_left, -mouseRotSpd * Input::Mouse::dy);
//             m_playerCameraComp->m_transform->m_orientation *= q;
//         }
//         
//         
//         //yaw/////////////////////////////////////
//         m_playerCameraComp->m_yawAngle += mouseRotSpd * Input::Mouse::dx;
//         kep::Quaternion q;
//         q.setEuler(m_playerCameraComp->m_up, mouseRotSpd * Input::Mouse::dx);
//         m_playerCameraComp->m_transform->m_orientation *= q;
//     
//         kep::Quaternion moveQuat;
//         moveQuat.setEuler(m_playerCameraComp->m_up, m_playerCameraComp->m_yawAngle);
//         kep::Matrix3 moveMat;
//         moveMat.setOrientation(moveQuat);
//         kep::Vector3 moveDir(0,0,-1);
//         moveDir = moveMat * moveDir;
//         
//         
// 
//         
//         //printf("comp: %f\n", moveDirVelComp);
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_U, Input::Keyboard::KeyboardAction::HELD))
        {
            m_agentBody_ref ->m_rigidBody->addForce(kep::Vector3(0,0,100));
        }
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_J, Input::Keyboard::KeyboardAction::HELD))
        {
            m_agentBody_ref ->m_rigidBody->addForce(kep::Vector3(0,0,-100));
        }
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_H, Input::Keyboard::KeyboardAction::HELD))
        {
            m_agentBody_ref ->m_rigidBody->addForce(kep::Vector3(100,0,0));
        }
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_K, Input::Keyboard::KeyboardAction::HELD))
        {
            m_agentBody_ref ->m_rigidBody->addForce(kep::Vector3(-100,0,0));
        }
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_SPACE, Input::Keyboard::KeyboardAction::PRESSED))
        {
            printf("Looking for loc\n");
            deway::Loc loc;
            if(m_agent_ref->m_pathFinder->getLoc(&loc, m_agent_ref->m_transform->m_position) == 0)
            {
                printf("found loc \n");
                //m_triGen_ref->clear();
                m_triGen_ref->m_verticies.clear();
                m_triGen_ref->addTri(loc.tri->vertex[0]->pos, loc.tri->vertex[1]->pos, loc.tri->vertex[2]->pos);
                m_triGen_ref->gen();
            }
        }


        
}
void Empty::render()
{
}
