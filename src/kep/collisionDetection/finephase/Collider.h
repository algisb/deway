#ifndef COLLIDER_H_
#define COLLIDER_H_
#include "../../Core.h"
#include "CollisionData.h"
namespace kep
{
    class MeshCollider;
    class OBBCollider;
    class HalfPlaneCollider;
    class SphereCollider;
    class RigidBody;
    class Collider
    {
    public:
        RigidBody * rigidBody;//set inside rigidBody constructor
        Matrix4 offset;//offset from rigidBody position
        Matrix4 transform;//offset + rigidBody position
        real maxReach;//used for generating a bounding volume
        
        Collider(Matrix4 _offset = Matrix4());
        virtual ~Collider()=0;
        virtual void update();
        virtual int collides(Collider * _c, CollisionData * _collisionData)=0;
        
        virtual int collides(SphereCollider * _c, CollisionData * _collisionData)=0;
        virtual int collides(HalfPlaneCollider * _c, CollisionData * _collisionData)=0;
        virtual int collides(OBBCollider * _c, CollisionData * _collisionData)=0;
        virtual int collides(MeshCollider * _c, CollisionData * _collisionData)=0;
    };
};

#endif //COLLIDER_H_
