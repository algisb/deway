#include "BoundingSphere.h"
#include "../../RigidBody.h"
#include "kep/collisionDetection/finephase/Collider.h"
using namespace kep;

BoundingSphere::BoundingSphere(RigidBody * _rigidBody, real _radius)
{
    rigidBody = _rigidBody;
    radius = _radius;
}

BoundingSphere::~BoundingSphere()
{
}

bool BoundingSphere::overlaps(BoundingSphere *_other)
{

    real distanceSquared = (rigidBody->collider->transform.getAxisVector(3) - _other->rigidBody->collider->transform.getAxisVector(3)).squareMagnitude();
    //real distanceSquared = (*rigidBody->position - *_other->rigidBody->position).squareMagnitude();
    return distanceSquared < (radius+_other->radius)*(radius+_other->radius);
}
