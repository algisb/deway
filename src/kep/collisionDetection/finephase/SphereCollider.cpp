#include "SphereCollider.h"
#include "HalfPlaneCollider.h"
#include "MeshCollider.h"

using namespace kep;

SphereCollider::SphereCollider(Matrix4 _offset, real _radius) : Collider(_offset)
{
    radius = _radius;
    maxReach = radius;
}
SphereCollider::~SphereCollider()
{
}
int SphereCollider::collides(Collider * _c, CollisionData * _collisionData)
{
    return _c->collides(this, _collisionData);
}

int SphereCollider::collides(SphereCollider * _c, CollisionData * _collisionData)
{
    Vector3 positionOne = _c->rigidBody->collider->transform.getAxisVector(3);
    Vector3 positionTwo = rigidBody->collider->transform.getAxisVector(3);
    Vector3 midline = positionOne - positionTwo;
    real size = midline.magnitude();
    real totalR = _c->radius+radius;
    if (size <= 0.0f || size >= totalR)
    {
        return 0;
    }
    
    Contact contact;
    contact.body[0] = rigidBody;
    contact.body[1] = _c->rigidBody;
    contact.normal = midline * (((real)1.0)/size);
    contact.position = positionOne + midline * (real)0.5;
    contact.penetration = (totalR - size);
    _collisionData->contacts.push_back(contact);
    return 1;
}
int SphereCollider::collides(HalfPlaneCollider * _c, CollisionData * _collisionData)
{
    kep::Vector3 normalTr = Matrix3(_c->transform) * _c->normal;
    Vector3 position = transform.getAxisVector(3) - _c->transform.getAxisVector(3);
    real ballDistance = normalTr * position - radius;
    //printf("%f \n", ballDistance);
    if (ballDistance >= 0 || 
        std::abs(ballDistance) > radius)//no contact when on the other side of the plane 
        return 0;
    
    Contact contact;
    contact.body[0] = rigidBody;
    contact.body[1] = _c->rigidBody;
    contact.normal = normalTr;
    contact.position = transform.getAxisVector(3) - normalTr * (ballDistance + radius);
    contact.penetration = -ballDistance;
    _collisionData->contacts.push_back(contact);
    return 1;
}
int SphereCollider::collides(OBBCollider * _c, CollisionData * _collisionData)
{

    return 0;
}

int SphereCollider::collides(MeshCollider * _c, CollisionData * _collisionData)
{
    return _c->collides(this,_collisionData);//defined in MeshCollider
}

