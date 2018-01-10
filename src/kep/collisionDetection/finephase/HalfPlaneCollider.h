#ifndef HALFPLANECOLLIDER_H_
#define HALFPLANECOLLIDER_H_
#include "Collider.h"
namespace kep
{
    class MeshCollider;
    class OBBCollider;
    class SphereCollider;
    class HalfPlaneCollider : public Collider
    {
    public:
        kep::Vector3 normal;
        HalfPlaneCollider(Matrix4 _offset = Matrix4(), kep::Vector3 _normal = kep::Vector3(0.0f, 1.0f, 0.0f));
        ~HalfPlaneCollider();
        virtual int collides(Collider * _c, CollisionData * _collisionData);

        virtual int collides(SphereCollider * _c, CollisionData * _collisionData);
        virtual int collides(HalfPlaneCollider * _c, CollisionData * _collisionData);
        virtual int collides(OBBCollider * _c, CollisionData * _collisionData);
        virtual int collides(MeshCollider * _c, CollisionData* _collisionData);
    };
};

#endif //HALFPLANECOLLIDER_H_
