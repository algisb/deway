#include "Empty.h"
#include "world/World_0.h"
#include "Input.h"
#include "Time.h"
#include <Core.h>
#include "deway/pathfinder/PathFinder.h"
#include "deway/pathfinder/Loc.h"
#include "deway/TriangleO.h"
#include "deway/Vertex.h"
#include "deway/pathfinder/Ray.h"
#include "deway/pathfinder/Loc.h"
#include "deway/TriangleO.h"

using namespace kelp;
using namespace deway;
Empty::Empty(Entity * _playerCamera, KePhys * _agentBody, Agent * _agent, MeshGen * _trigen, MeshGenLine * _testLine)
{
    kekX = -8.0f;
    m_playerCameraEntity = _playerCamera;
    m_agentBody_ref = _agentBody;
    m_agent_ref = _agent;
    m_triGen_ref = _trigen;
    m_testLine_ref = _testLine;
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
        m_agentBody_ref->m_rigidBody->velocity = kep::Vector3(m_agentBody_ref->m_rigidBody->velocity.x, m_agentBody_ref->m_rigidBody->velocity.y+15.0f, m_agentBody_ref->m_rigidBody->velocity.z);
// Gets the location of the agent body
//         printf("Looking for loc\n");
//         deway::Loc loc;
//         if(m_agent_ref->m_pathFinder->getLoc(&loc, m_agent_ref->m_transform->m_position) == 0)
//         {
//             printf("found loc \n");
//             //m_triGen_ref->clear();
//             m_triGen_ref->m_verticies.clear();
//             m_triGen_ref->addTri(loc.tri->vertex[0]->pos, loc.tri->vertex[1]->pos, loc.tri->vertex[2]->pos);
//             m_triGen_ref->gen();
//         }
    }
    if(Input::Mouse::is(Input::Mouse::MOUSE_BUTTON_LEFT,Input::Mouse::MouseAction::PRESSED))
    {

        deway::Ray mouseRay = genMouseRay();
        
        kep::Vector3 p2 = mouseRay.s + (mouseRay.d * 100.0f);
//         m_testLine_ref->m_verticies.clear();
//         m_testLine_ref->addLine(mouseRay.s, p2);
//         m_testLine_ref->gen();
        
        deway::Loc mouseRayLoc = getMouseRayLoc(mouseRay);
        
        if(mouseRayLoc.tri != NULL)
        {
            m_triGen_ref->m_verticies.clear();
            m_triGen_ref->addTri(mouseRayLoc.tri->vertex[0]->pos, mouseRayLoc.tri->vertex[1]->pos, mouseRayLoc.tri->vertex[2]->pos);
            m_triGen_ref->gen();
            
            m_agent_ref->genPath(&mouseRayLoc);
            
            ////////////////////////
            if(m_agent_ref->m_path.size() > 1)
            {
                m_testLine_ref->m_verticies.clear();
                for(uint i = 0; i < m_agent_ref->m_path.size()-1; i++)
                {
                    m_testLine_ref->addLine(m_agent_ref->m_path[i], m_agent_ref->m_path[i+1]);
                }
                m_testLine_ref->gen();
            }
        }
        else
        {
            printf("Selected location is invalid\n");
        }
        

        
    }


        
}
void Empty::render()
{
}

deway::Ray Empty::genMouseRay()
{
    //GEN RAY
    float x_hwc = (Input::Mouse::x * 2.0f)/Config::s_windowWidth -1.0f;
    float y_hwc = 1.0f - (2.0f * Input::Mouse::y) / Config::s_windowHeight;


    kep::Matrix4 vI;
    vI.setOrientationAndPos(m_playerCameraComp->m_transform->m_orientation, kep::Vector3());
    kep::Matrix4 pI = m_playerCameraComp->m_projectionMat.inverse();

    kep::Vector4 rayClip(x_hwc, y_hwc, -1.0f, 1.0f);

    kep::Vector4 rayEye = pI * rayClip;
    rayEye = kep::Vector4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    kep::Vector3 rayWorld = kep::Matrix3(vI) * kep::Vector3(rayEye.x, rayEye.y, rayEye.z);

    kep::Vector3 rayWorld3(rayWorld.x, rayWorld.y, rayWorld.z);
    rayWorld3.normalize();
    
    return deway::Ray(m_playerCameraComp->m_transform->m_position, rayWorld3);
}

deway::Loc Empty::getMouseRayLoc(deway::Ray _mouseRay)
{
    deway::Loc loc(NULL);
    
    kep::Vector3 p(0.0f ,0.0f, 0.0f);
    
    deway::TriangleO * t = NULL;
    
    for(uint i = 0; i<m_agent_ref->m_pathFinder->m_navMeshSort.size(); i++)
        if(deway::PathFinder::rayTriangleMT97(&_mouseRay, m_agent_ref->m_pathFinder->m_navMeshSort[i], &p) == 1)
        {
            t = m_agent_ref->m_pathFinder->m_navMeshSort[i];
            break;
        }
        
    if(t != NULL)
    {
        loc.tri = t;
        loc.pos = kep::Vector3(p.x, p.y+1.0f, p.z);// shift y pos up by 1
    }
    return loc;
}

