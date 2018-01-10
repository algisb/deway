#ifndef OBBCOLLIDER_H_
#define OBBCOLLIDER_H_
#include "Collider.h"
namespace kep
{
    class MeshCollider;
    class HalfPlaneCollider;
    class SphereCollider;
    class OBBCollider : public Collider
    {
    public:
        kep::Vector3 halfSize;
        OBBCollider(Matrix4 _offset, Vector3 _halfSize);
        ~OBBCollider();
        real transformToAxis(OBBCollider * _box, Vector3 * _axis);
        bool overlapOnAxis(OBBCollider * _one, OBBCollider * _two, Vector3 * _axis);

        
        virtual int collides(Collider * _c, CollisionData * _collisionData);
        
        virtual int collides(SphereCollider * _c, CollisionData * _collisionData);
        virtual int collides(HalfPlaneCollider * _c, CollisionData * _collisionData);
        virtual int collides(OBBCollider * _c, CollisionData * _collisionData);
        virtual int collides(MeshCollider * _c, CollisionData * _collisionData);
        
    };
};
#endif //OBBCOLLIDER_H_
