#include "Camera.h"
#include "Input.h"
#include "Time.h"
using namespace kelp;

Camera::Camera(kep::Vector3 _up, kep::Matrix4 _projectionMat, bool _possessed)
{
    m_up = _up;
    m_projectionMat = _projectionMat;
    m_possessed = _possessed;
    m_pitchAngle = 0.0f;
    m_yawAngle = 0.0f;
}
Camera::~Camera()
{
}

void Camera::init()
{
    if(m_owner->m_world->m_renderCamera == NULL)
        m_owner->m_world->m_renderCamera = this;
    m_transform = m_owner->getComponent<Transform>();
}
void Camera::update()
{

    //m_transform->m_orientation.normalize();
    m_viewMat = m_transform->m_modelMat; //setOrientationAndPos(m_transform->m_orientation, m_transform->m_position);
    m_viewMat.invert();
    m_viewMat = m_viewMat.transpose(); //cuz opengl is idgaf-major
    
    m_front =  kep::Matrix3(m_viewMat) * kep::Vector3(0.0f, 0.0f, -1.0f);
    m_front.normalize();
    
    kep::Vector3 tmpLeft = kep::cross(m_up, m_front);
    if(!(tmpLeft == kep::Vector3(0.0f,0.0f,0.0f)))
    {
        m_left = tmpLeft;
        m_left.normalize();
    }
    if(m_possessed)
        possessedControls();
}

void Camera::render()
{
}

void Camera::possessedControls()
{
    //Camera * rc = m_owner->m_world->m_renderCamera;
    const float movSpd = 20.0f * Time::s_deltaT;
    if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_W, Input::Keyboard::KeyboardAction::HELD))
        m_transform->m_position += m_front * movSpd;
    if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_S, Input::Keyboard::KeyboardAction::HELD))
        m_transform->m_position += m_front * -movSpd;
    if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_A, Input::Keyboard::KeyboardAction::HELD))
        m_transform->m_position += m_left * movSpd;
    if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_D, Input::Keyboard::KeyboardAction::HELD))
        m_transform->m_position += m_left * -movSpd;
    if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_E, Input::Keyboard::KeyboardAction::HELD))
        m_transform->m_position += m_up * movSpd;
    if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_C, Input::Keyboard::KeyboardAction::HELD))
        m_transform->m_position += m_up * -movSpd;
    
    
    const float rotSpd = 70.0f * Time::s_deltaT;
    
    //Keyboard rotation
    if(!Input::Mouse::joyStickMode)
    {
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_UP, Input::Keyboard::KeyboardAction::HELD))
        {
            
            
            float tmpAng = m_pitchAngle;
            tmpAng = tmpAng - rotSpd;
            if(tmpAng > -89.0f)
            {
                m_pitchAngle = tmpAng;
                kep::Quaternion q;
                q.setEuler(m_left, -rotSpd);
                m_transform->m_orientation *= q;
                
            }
        }
        
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_DOWN, Input::Keyboard::KeyboardAction::HELD))
        {
            float tmpAng = m_pitchAngle;
            tmpAng = tmpAng + rotSpd;
            
            if(tmpAng < 89.0f)
            {
                m_pitchAngle = tmpAng;
                kep::Quaternion q;
                q.setEuler(m_left, rotSpd);
                m_transform->m_orientation *= q;
            }
        }
        kep::Quaternion qq;
        qq.normalize();
        m_transform->m_orientation *= qq;
        
        
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_LEFT, Input::Keyboard::KeyboardAction::HELD))
        {
            //m_transform->m_orientation.addScaledVector(kep::Vector3(0.0f, 1.0f, 0.0f), 0.01f);
            kep::Quaternion q;
            q.setEuler(m_up, -rotSpd);
            m_transform->m_orientation *= q;
        }
        //printf("%f %f %f %f\n", m_transform->m_orientation.i, m_transform->m_orientation.j, m_transform->m_orientation.k, m_transform->m_orientation.r);
        if(Input::Keyboard::is(Input::Keyboard::KeyboardKey::KEY_RIGHT, Input::Keyboard::KeyboardAction::HELD))
        {
            //rc->m_transform->m_orientation.addScaledVector(kep::Vector3(0.0f, 1.0f, 0.0f), 0.01f);
            kep::Quaternion q;
            q.setEuler(m_up, rotSpd);
            m_transform->m_orientation *= q;
        }
    }
    else
    {
        const float mouseRotSpd = rotSpd *0.1f;
        float tmpAng = m_pitchAngle;
        tmpAng += -mouseRotSpd * Input::Mouse::dy;
        
        if(tmpAng < 90.0f && tmpAng > -90.0f)
        {
            m_pitchAngle = tmpAng;
            kep::Quaternion q;
            q.setEuler(m_left, -mouseRotSpd * Input::Mouse::dy);
            m_transform->m_orientation *= q;
        }
        
        
        //Mouse rotation
        kep::Quaternion q;
        q.setEuler(m_up, mouseRotSpd * Input::Mouse::dx);
        m_transform->m_orientation *= q;
        
    }

}
void Camera::setAsRenderCamera()
{
    m_owner->m_world->m_renderCamera->m_possessed = false;
    m_owner->m_world->m_renderCamera = this;
}

