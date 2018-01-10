#include "OBBCollider.h"
using namespace kep;
OBBCollider::OBBCollider(Matrix4 _offset, Vector3 _halfSize) : Collider(_offset)
{
    halfSize = _halfSize;
    maxReach = halfSize.magnitude();
}

OBBCollider::~OBBCollider()
{
}

real OBBCollider::transformToAxis(OBBCollider * _box, Vector3 * _axis)
{
    return
    _box->halfSize.x * real_abs(*_axis * _box->transform.getAxisVector(0)) +
    _box->halfSize.y * real_abs(*_axis * _box->transform.getAxisVector(1)) +
    _box->halfSize.z * real_abs(*_axis * _box->transform.getAxisVector(2));

}

bool OBBCollider::overlapOnAxis(OBBCollider*  _one, OBBCollider * _two, Vector3 * _axis)
{
    // Project the half-size of one onto axis.
    real oneProject = transformToAxis(_one, _axis);
    real twoProject = transformToAxis(_two, _axis);
    // Find the vector between the two centers.
    Vector3 toCenter = _two->transform.getAxisVector(3) - _one->transform.getAxisVector(3);
    // Project this onto the axis.
    real distance = real_abs(toCenter * (*_axis));
    // Check for overlap.
    return (distance < oneProject + twoProject);
}

int OBBCollider::collides(Collider* _c, CollisionData* _collisionData)
{
    return _c->collides(this, _collisionData);
}
int OBBCollider::collides(SphereCollider* _c, CollisionData* _collisionData)
{
    return 0;
}
int OBBCollider::collides(HalfPlaneCollider* _c, CollisionData* _collisionData)
{
    return 0;
}

int OBBCollider::collides(OBBCollider* _c, CollisionData* _collisionData)
{
    //TEST_OVERLAP(transform.getAxisVector(0));
    Vector3 axis0[3] = {transform.getAxisVector(0),transform.getAxisVector(1), transform.getAxisVector(2)};
    Vector3 axis1[3] = {_c->transform.getAxisVector(0),_c->transform.getAxisVector(1), _c->transform.getAxisVector(2)};
    Vector3 axisC[9] = {axis0[0] % axis1[0],
                        axis0[0] % axis1[1],
                        axis0[0] % axis1[2],
                        axis0[1] % axis1[0],
                        axis0[1] % axis1[1],
                        axis0[1] % axis1[2],
                        axis0[2] % axis1[0],
                        axis0[2] % axis1[1],
                        axis0[2] % axis1[2]};
    
    if(     overlapOnAxis(this, _c, &axis0[0]) &&
            overlapOnAxis(this, _c, &axis0[1]) &&
            overlapOnAxis(this, _c, &axis0[2]) &&
            
            overlapOnAxis(this, _c, &axis1[0]) &&
            overlapOnAxis(this, _c, &axis1[1]) &&
            overlapOnAxis(this, _c, &axis1[2]) &&
                                            
            overlapOnAxis(this, _c, &axisC[0]) &&
            overlapOnAxis(this, _c, &axisC[1]) &&
            overlapOnAxis(this, _c, &axisC[2]) &&
            overlapOnAxis(this, _c, &axisC[3]) &&
            overlapOnAxis(this, _c, &axisC[4]) &&
            overlapOnAxis(this, _c, &axisC[5]) &&
            overlapOnAxis(this, _c, &axisC[6]) &&
            overlapOnAxis(this, _c, &axisC[7]) &&
            overlapOnAxis(this, _c, &axisC[8])
    )
    {
//         Contact contact;
//         Vector3 positionOne = _c->rigidBody->collider->transform.getAxisVector(3);
//         Vector3 positionTwo = rigidBody->collider->transform.getAxisVector(3);
//         Vector3 midline = positionOne - positionTwo;
//         real size = midline.magnitude();
//         
//         contact.body[0] = _c->rigidBody;
//         contact.body[1] = rigidBody;
//         contact.normal = midline * (((real)1.0)/size);
//         contact.position = positionOne + midline * (real)0.5;
//         contact.penetration = 0;
//         _collisionData->contacts.push_back(contact);
        return 1;
    }
    else
        return 0;
}
int OBBCollider::collides(MeshCollider* _c, CollisionData* _collisionData)
{
    return 0;
}


