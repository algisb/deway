#include "ForceGenerator.h"
using namespace kep;

ForceGenerator::ForceGenerator()
{
}
ForceGenerator::~ForceGenerator()
{
    
}


ForceRegistry::ForceRegistry()
{
}

void ForceRegistry::updateForces(real _duration)
{
    for(int i = 0; i<m_registrations.size(); i++)
    {
        ForceRegistration * rtmp = &m_registrations[i];
        rtmp->fg->updateForce(rtmp->rigidBody, _duration);
    }
}


void ForceRegistry::add(RigidBody * _rigidBody, ForceGenerator * _fg)
{
    ForceRegistration pfr;
    pfr.fg = _fg;
    pfr.rigidBody = _rigidBody;
    m_registrations.push_back(pfr);
}

////////////////////
//Derived generators
////////////////////
//gravity
Gravity::Gravity(const Vector3 & _gravity)
{
    m_gravity = _gravity;
}
void Gravity::updateForce(RigidBody * _rigidBody, real _duration)
{
    if(_rigidBody->hasFiniteMass())
        _rigidBody->addForce((m_gravity)* _rigidBody->getMass());
}

//Drag
LinearDrag::LinearDrag(real _k1, real _k2)
{
    m_k1 = _k1;
    m_k2 = _k2;
}
void LinearDrag::updateForce(RigidBody * _rigidBody, real _duration)
{
    Vector3 force;
    force = _rigidBody->velocity;
    
    real dragCoeff  = force.magnitude();

    dragCoeff = m_k1 * dragCoeff + m_k2 * dragCoeff * dragCoeff;
    force.normalize();
    force *= -dragCoeff;
    _rigidBody->addForce(force);


    
    
}

AngularDrag::AngularDrag(real _k1, real _k2)
{
    m_k1 = _k1;
    m_k2 = _k2;
}

void AngularDrag::updateForce(RigidBody * _rigidBody, real _duration)
{
    Vector3 torque;
    torque = _rigidBody->angularVelocity;
    
    real dragCoeff = torque.magnitude();
    dragCoeff = m_k1 * dragCoeff + m_k2 * dragCoeff * dragCoeff;
    torque.normalize();
    torque *= -dragCoeff;
    
    _rigidBody->addTorque(torque);
}

Spring::Spring(const Vector3 & _localConnectionPoint, const Vector3 & _otherConnectionPoint, real _springConstant, real _restLength)
{
    m_localConnectionPoint = _localConnectionPoint;
    m_otherConnectionPoint = _otherConnectionPoint;
    m_springConstant = _springConstant;
    m_restLength = _restLength;
}

void Spring::updateForce(RigidBody * _rigidBody, real _duration)
{
    Vector3 lws = localToWorld(m_localConnectionPoint, _rigidBody->transformMatrix);
    Vector3 ows = m_otherConnectionPoint;
    
    Vector3 force = lws - ows;
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - m_restLength);
    magnitude *= m_springConstant;
    force.normalize();
    force *= -magnitude;
    _rigidBody->addForceAtPoint(force, lws);
}
