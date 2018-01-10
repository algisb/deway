#ifndef MESHCOLLIDER_H_
#define MESHCOLLIDER_H_
#include "Collider.h"

namespace kep
{
    class Triangle
    {
    public:
        kep::Vector3 p[3];
        kep::Vector3 n;
        Triangle(kep::Vector3 _p0 = kep::Vector3(0.0f, 0.0f, 0.0f), 
                 kep::Vector3 _p1 = kep::Vector3(0.0f, 0.0f, 0.0f), 
                 kep::Vector3 _p2 = kep::Vector3(0.0f, 0.0f, 0.0f), 
                 kep::Vector3 _n = kep::Vector3(0.0f ,0.0f, 0.0f));
        ~Triangle();
    };
    class Ray
    {
    public:
        kep::Vector3 s;
        kep::Vector3 d;
        Ray(kep::Vector3 _source = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _direction = kep::Vector3(0.0f, 0.0f, 1.0f));
        ~Ray();
        
    };

    
    class MeshCollider : public Collider
    {
    public:
        
        int numTriangles;
        Triangle * triangles;
        Triangle * tTriangles;
        
        Matrix4 scaleMat;
        
        MeshCollider(Matrix4 _offset, float * _dataV, float * _dataN, int _numVertex, Vector3 _scale);
        ~MeshCollider();
        void update();
        int spherePlane(Vector3 _spherePosition,
                        float _sphereRadius,
                        Vector3 _planeNormal,
                        Vector3 _planePosition,
                        
                        Vector3 * _contactPosition,
                        float * _penetration);
        int rayTriangleMT97(Ray * _ray, Triangle * _triangle,  Vector3 * o_point);
        int inTriangle(Triangle _t,  Vector3 _p);
        virtual int collides(Collider * _c, CollisionData * _collisionData);
        
        virtual int collides(SphereCollider * _c, CollisionData * _collisionData);
        virtual int collides(HalfPlaneCollider * _c, CollisionData * _collisionData);
        virtual int collides(OBBCollider * _c, CollisionData * _collisionData);
        virtual int collides(MeshCollider * _c, CollisionData * _collisionData);
    };
};

#endif //MESHCOLLIDER_H_
