#include "Transform.h"

using namespace kelp;

Transform::Transform(kep::Vector3 _position, kep::Quaternion _orientation, kep::Vector3 _scale) : Component()
{
    m_position = _position;
    m_orientation = _orientation;
    m_scale = _scale;
}
Transform::~Transform()
{
    
}

void Transform::init()
{
}

void Transform::update()
{
    m_orientation.normalize();
    m_modelMat.setOrientationAndPos(m_orientation, m_position);
    m_modelMatUnscaled = m_modelMat;
    m_modelMat = m_modelMat * kep::Matrix4(m_scale.x, 0.0f, 0.0f, 0.0f,
                                           0.0f, m_scale.y, 0.0f, 0.0f,
                                           0.0f, 0.0f, m_scale.z, 0.0f,
                                           0.0f, 0.0f, 0.0f, 1.0f);
    Entity * par = m_owner->m_parent;    
    while(par != NULL)
    {
        m_modelMat = par->m_transform->m_modelMatUnscaled * m_modelMat;
        par = par->m_parent;
    }
            
}

void Transform::dump()
{
    printf("pos: %f %f %f\n", m_position.x, m_position.y, m_position.z);
}
