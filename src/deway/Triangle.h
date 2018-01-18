#ifndef TRIANGLE_DEWAY_H_
#define TRIANGLE_DEWAY_H_
#include "Core.h"
namespace deway
{
    class AABB;
    class Triangle
    {
    public:
        kep::Vector3 p[3];
        kep::Vector3 n;
        AABB * aabb;
        Triangle(kep::Vector3 _p0 = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _p1 = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _p2 = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _n = kep::Vector3(0.0f ,0.0f, 0.0f));
        ~Triangle();
        void genAABB();
    };
};
#endif //TRIANGLE_DEWAY_H_
