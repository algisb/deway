#include "HalfPlaneCollider.h"
#include "SphereCollider.h"
using namespace kep;
HalfPlaneCollider::HalfPlaneCollider(Matrix4 _offset, Vector3 _normal) : Collider(_offset)
{
    normal = _normal;
    maxReach = 14.14f;
}
HalfPlaneCollider::~HalfPlaneCollider()
{
}
int HalfPlaneCollider::collides(Collider * _c, CollisionData * _collisionData)
{
    return _c->collides(this,_collisionData);
}

int HalfPlaneCollider::collides(SphereCollider* _c, CollisionData* _collisionData)
{
    return _c->collides(this,_collisionData);//defined in sphere collider
}

int HalfPlaneCollider::collides(HalfPlaneCollider* _c, CollisionData* _collisionData)//plane plane collision not needed
{
    return 0;
}

int HalfPlaneCollider::collides(OBBCollider * _c, CollisionData * _collisionData)
{
    return 0;
}

int HalfPlaneCollider::collides(MeshCollider * _c, CollisionData * _collisionData)
{
    return 0;
}


