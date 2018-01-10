#include "RigidBody.h"
#include "collisionDetection/broadphase/BoundingSphere.h"
#include "collisionDetection/finephase/SphereCollider.h"
using namespace kep;


RigidBody::RigidBody(Vector3 * _position, Quaternion * _orientation, bool _externPO, real _mass, Collider * _collider, bool _noRotation)
{
    externPO = _externPO;
    noRotation = _noRotation;
    
    if(!externPO)
    {
        position = new Vector3();
        *position = *_position;
        
        orientation = new Quaternion();
        *orientation = *_orientation;
    }
    else
    {
        position = _position;
        orientation = _orientation;
    }
    
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    acceleration = Vector3(0.0f, 0.0f, 0.0f);
    forceAccum = Vector3(0.0f, 0.0f, 0.0f);
    

    angularVelocity = Vector3(0.0f, 0.0f, 0.0f);
    torqueAccum = Vector3(0.0f, 0.0f, 0.0f);
    
    
    if(_mass > 0.0f)
        inverseMass = 1.0f/_mass;
    else
        inverseMass = 0.0f;
    
    //TODO this will depend on the shape of the object 
    inverseInertiaTensor = Matrix3(_mass,0,0,
                                   0,_mass,0,
                                   0,0,_mass);
    
    transformMatrix = Matrix4(1,0,0,0,
                              0,1,0,0,
                              0,0,1,0,
                              0,0,0,1
    );
    
    collider = _collider;
    if(collider != NULL)
    {
        collider->rigidBody = this;
        boundingVolume = new BoundingSphere(this, collider->maxReach);//generates
    }
    else
        boundingVolume = new BoundingSphere(this, 1.0f);
    calculateDerivedData();
}
RigidBody::~RigidBody()
{
    if(!externPO)
    {
        delete position;
        delete orientation;
    }
    delete boundingVolume;
    delete collider;
}

void RigidBody::calculateDerivedData()
{
    orientation->normalize();
    transformMatrix.setOrientationAndPos(*orientation, *position);
    transformInertiaTensor(inverseInertiaTensorWorld,
                           *orientation,
                           inverseInertiaTensor,
                           transformMatrix);
    //collider has a separate transform
    if(collider != NULL)
    {
        collider->transform = collider->offset * transformMatrix;
        collider->update();
    }
    //transformMatrix = transformMatrix.transpose();//needed because openGL ..
}

void RigidBody::setInertiaTensor(const Matrix3 &_inertiaTensor)
{
    inverseInertiaTensor.setInverse(_inertiaTensor);
}

void RigidBody::transformInertiaTensor(Matrix3 &_iitWorld,
                                    const Quaternion &_q,
                                    const Matrix3 &_iitBody,
                                    const Matrix4 &_rotmat)
{
    real t4 = _rotmat.data[0]*_iitBody.data[0]+
    _rotmat.data[1]*_iitBody.data[3]+
    _rotmat.data[2]*_iitBody.data[6];
    real t9 = _rotmat.data[0]*_iitBody.data[1]+
    _rotmat.data[1]*_iitBody.data[4]+
    _rotmat.data[2]*_iitBody.data[7];
    real t14 = _rotmat.data[0]*_iitBody.data[2]+
    _rotmat.data[1]*_iitBody.data[5]+
    _rotmat.data[2]*_iitBody.data[8];
    real t28 = _rotmat.data[4]*_iitBody.data[0]+
    _rotmat.data[5]*_iitBody.data[3]+
    _rotmat.data[6]*_iitBody.data[6];
    real t33 = _rotmat.data[4]*_iitBody.data[1]+
    _rotmat.data[5]*_iitBody.data[4]+
    _rotmat.data[6]*_iitBody.data[7];
    real t38 = _rotmat.data[4]*_iitBody.data[2]+
    _rotmat.data[5]*_iitBody.data[5]+
    _rotmat.data[6]*_iitBody.data[8];
    
    real t52 = _rotmat.data[8]*_iitBody.data[0]+
    _rotmat.data[9]*_iitBody.data[3]+
    _rotmat.data[10]*_iitBody.data[6];
    real t57 = _rotmat.data[8]*_iitBody.data[1]+
    _rotmat.data[9]*_iitBody.data[4]+
    _rotmat.data[10]*_iitBody.data[7];
    real t62 = _rotmat.data[8]*_iitBody.data[2]+
    _rotmat.data[9]*_iitBody.data[5]+
    _rotmat.data[10]*_iitBody.data[8];

    _iitWorld.data[0] = t4*_rotmat.data[0]+
    t9*_rotmat.data[1]+
    t14*_rotmat.data[2];
    _iitWorld.data[1] = t4*_rotmat.data[4]+
    t9*_rotmat.data[5]+
    t14*_rotmat.data[6];
    _iitWorld.data[2] = t4*_rotmat.data[8]+
    t9*_rotmat.data[9]+
    t14*_rotmat.data[10];
    _iitWorld.data[3] = t28*_rotmat.data[0]+
    t33*_rotmat.data[1]+
    t38*_rotmat.data[2];
    _iitWorld.data[4] = t28*_rotmat.data[4]+
    t33*_rotmat.data[5]+
    t38*_rotmat.data[6];
    _iitWorld.data[5] = t28*_rotmat.data[8]+
    t33*_rotmat.data[9]+
    t38*_rotmat.data[10];
    _iitWorld.data[6] = t52*_rotmat.data[0]+
    t57*_rotmat.data[1]+
    t62*_rotmat.data[2];
    _iitWorld.data[7] = t52*_rotmat.data[4]+
    t57*_rotmat.data[5]+
    t62*_rotmat.data[6];
    _iitWorld.data[8] = t52*_rotmat.data[8]+
    t57*_rotmat.data[9]+
    t62*_rotmat.data[10];

    

}

void RigidBody::addForce(const Vector3 &_force)
{
    forceAccum += _force;
}
void RigidBody::addTorque(const Vector3 &_torque)
{
    torqueAccum += _torque;
}

void RigidBody::addForceAtPoint(const Vector3 &_force,
                                const Vector3 &_point)
{
    //calculateDerivedData();
    Vector3 pt = _point;
    pt -= *position;

    forceAccum += _force;
    torqueAccum += pt % _force;
}

void RigidBody::addForceAtBodyPoint(const Vector3 &_force, const Vector3 &_point)
{
    calculateDerivedData();
    Vector3 pt = localToWorld(_point, transformMatrix);
    addForceAtPoint(_force, pt);
}

void RigidBody::integrate(real _duration)
{
    //update position
    position->addScaledVector(velocity, _duration);
    
    //update acceleration
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);
        
    //update linear velocity 
    velocity.addScaledVector(lastFrameAcceleration, _duration);
    
    
    
    Vector3 angularAcceleration = inverseInertiaTensorWorld * torqueAccum;
    angularVelocity.addScaledVector(angularAcceleration, _duration);
    
    if(!noRotation)
        orientation->addScaledVector(angularVelocity, _duration);
    //add drag
    //velocity *= real_pow(damping, _duration);//real_pow(m_damping, _duration);
    
    calculateDerivedData();
    clearAccumulators();
}
void RigidBody::clearAccumulators()
{
    forceAccum = Vector3(0,0,0);
    torqueAccum = Vector3(0,0,0);
}

real RigidBody::getMass()
{
    return 1.0f/inverseMass;
}

bool RigidBody::hasFiniteMass()
{
    if(inverseMass > 0.0f)
        return true;
    else
        return false;
}
