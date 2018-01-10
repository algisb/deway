#include "MeshCollider.h"
#include "SphereCollider.h"
using namespace kep;

Triangle::Triangle(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _p2, kep::Vector3 _n)
{
    p[0] = _p0;
    p[1] = _p1;
    p[2] = _p2;
    n = _n;
}

Triangle::~Triangle()
{
}

Ray::Ray(kep::Vector3 _source, kep::Vector3 _direction)
{
    s = _source;
    d = _direction;
}

Ray::~Ray()
{
}



MeshCollider::MeshCollider(Matrix4 _offset, float* _dataV, float* _dataN, int _numVertex, Vector3 _scale)
{
    numTriangles = _numVertex/3;
    triangles = new Triangle[numTriangles];
    tTriangles = new Triangle[numTriangles];
    maxReach = 1.0f;
    
    scaleMat = Matrix4(_scale.x, 0.0f, 0.0f, 0.0f,
                            0.0f, _scale.y, 0.0f, 0.0f,
                            0.0f, 0.0f, _scale.z, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f
                           );
    for(int i = 0; i <_numVertex*3; i = i + 3)//maxReach calculated at the initilization if the mesh changes size at runtime the bounding volume will not follow
    {
        Vector3 v(_dataV[i+0],_dataV[i+1],_dataV[i+2]);
        float mag = (scaleMat*v).magnitude();
        if(mag > maxReach)
            maxReach = mag;
    }
    
    
    //printf("%f \n", maxReach);
    
    for(int i = 0; i <_numVertex*3; i = i + 9)
    {
        triangles[i/9] = Triangle(Vector3( _dataV[i+0], _dataV[i+1], _dataV[i+2]),
                            Vector3( _dataV[i+3], _dataV[i+4], _dataV[i+5]),
                            Vector3( _dataV[i+6], _dataV[i+7], _dataV[i+8]),
                            Vector3( _dataN[i+0], _dataN[i+1], _dataN[i+2]));
    }
}
MeshCollider::~MeshCollider()
{
    delete [] triangles;
    delete [] tTriangles;
}

void MeshCollider::update()
{
    Matrix4 tr = transform * scaleMat;
    for(int i = 0 ; i < numTriangles; i++)
    {
        tTriangles[i] = Triangle(tr * triangles[i].p[0],
                                 tr * triangles[i].p[1],
                                 tr * triangles[i].p[2],
                                 Matrix3(transform) * triangles[i].n);
    }
}

int MeshCollider::spherePlane(Vector3 _spherePosition,
                              float _sphereRadius,
                              Vector3 _planeNormal,
                              Vector3 _planePosition,
                              
                              Vector3 * _contactPosition,
                              float * _penetration
                             )
{
    Vector3 position = _spherePosition - _planePosition;
    real ballDistance = _planeNormal * position - _sphereRadius;
    if (ballDistance >= 0 ||
        std::abs(ballDistance) > _sphereRadius)//no contacts generated if on the other side of the plane 
        return 0;
    
    //*_contactPosition = position - _planeNormal * (ballDistance + _sphereRadius);
    *_contactPosition = _spherePosition - _planeNormal * (ballDistance + _sphereRadius);
    *_penetration = -ballDistance;
    return 1;
}

int MeshCollider::rayTriangleMT97(Ray * _ray, Triangle * _triangle,  Vector3 * o_point)// 64 flops
{
    kep::Vector3 edge1, edge2, pvec, tvec, qvec;
    float det, inv_det, u, v, w, t;
    edge1 = _triangle->p[1] - _triangle->p[0];// 3 flops
    edge2 = _triangle->p[2] - _triangle->p[0];// 3 flops
    
    pvec  = kep::cross(_ray->d, edge2);// 9 flops
    
    det = kep::dot(edge1, pvec);// 5 flops
    if(det == 0.0f)
        return 0;
    
    inv_det = 1.0f/det;// 1 flops
    
    tvec = _ray->s - _triangle->p[0];// 3 flops
    
    u = kep::dot(tvec, pvec) * inv_det;// 8 flops
    if(u < 0.0f || u > 1.0f)
        return 0;
    
    qvec = kep::cross(tvec, edge1);//9 flops
    
    v = kep::dot(_ray->d, qvec) * inv_det;// 8 flops
    if(v < 0.0f || v > 1.0f)
        return 0;
    
    w = u + v;//1 flop
    if(w < 0.0f || w > 1.0f)
        return 0;
    
    t = kep::dot(edge2, qvec) * inv_det;// 8 flops
    if(t < 0.0f)
        return 0;
    *o_point = _ray->s + _ray->d * t; // 6 flops
    return 1;;
}
int MeshCollider::inTriangle(Triangle _t,  Vector3 _p)// total 63 flops
{
    kep::Vector3 v0, v1, v2;
    float dot00, dot01, dot02, dot11, dot12, invDenom, u, v;
    
    v0 = _t.p[1] - _t.p[0];//3 flops

    v1 = _t.p[2] - _t.p[0];//3 flops

    v2 = _p - _t.p[0];// 3 flops
    
    dot00 = kep::dot(v0, v0);// 5 flops

    dot01 = kep::dot(v0, v1);// 5 flops

    dot02 = kep::dot(v0, v2);// 5 flops

    dot11 = kep::dot(v1, v1);// 5 flops

    dot12 = kep::dot(v1, v2);// 5 flops

    // Compute barycentric coordinates
    invDenom = 1 / (dot00 * dot11 - dot01 * dot01); // 4 flops
    u = (dot11 * dot02 - dot01 * dot12) * invDenom; // 4 flops
    v = (dot00 * dot12 - dot01 * dot02) * invDenom; // 4 flops
    // Check if point is in triangle

    if ((u >= 0.0f) && (v >= 0.0f) && (u <= 1.0f) && (v <= 1.0f) && (u + v <= 1.0f))
        return 1;
    else
        return 0;
}


int MeshCollider::collides(Collider* _c, CollisionData* _collisionData)
{
    return _c->collides(this, _collisionData);
}
int MeshCollider::collides(SphereCollider* _c, CollisionData* _collisionData)
{
    //printf("pre\n");
    int totalContacts = 0;
    for(int i = 0 ; i < numTriangles; i++)
    {
        Vector3 contactPosition;
        float penetration = 0.0f;
        Vector3 spherePosition = _c->transform.getAxisVector(3);
        
        int test = spherePlane(spherePosition,
                    _c->radius,
                    tTriangles[i].n, 
                    tTriangles[i].p[0], 
                    &contactPosition, 
                    &penetration);
        
        if(test == 0)
            continue;
        
        if(inTriangle(tTriangles[i], contactPosition) == 0)
            continue;
 //       printf("aft\n");
//         contactPosition.dump();
//         printf("tri %d:\n",i);
//         tTriangles[i].p[0].dump();
//         tTriangles[i].p[1].dump();
//         tTriangles[i].p[2].dump();
//         printf("\n");
        //kep::Vector3 cp;
        //Ray rc = Ray(spherePosition ,kep::Vector3(0,-1, 0));//(contactPosition-spherePosition).normalized()
        //if(rayTriangleMT97(&rc, &tTriangles[i], &cp) == 0)
        //    continue;
        Contact contact;
        contact.body[1] = rigidBody;
        contact.body[0] = _c->rigidBody;
        contact.normal = tTriangles[i].n;
        contact.position = contactPosition;
        contact.penetration = penetration;
        _collisionData->contacts.push_back(contact);
        totalContacts++;
        //break //uncommet to limit the mesh collider to generating only 1 contact
    }
    //printf("%d\n", totalContacts);
    return totalContacts;
}
int MeshCollider::collides(HalfPlaneCollider* _c, CollisionData* _collisionData)
{
    return 0;
}
int MeshCollider::collides(OBBCollider* _c, CollisionData* _collisionData)
{
    return 0;
}
int MeshCollider::collides(MeshCollider * _c, CollisionData * _collisionData)
{
    return 0;
}


