#ifndef TRIANGLE_DEWAY_H_
#define TRIANGLE_DEWAY_H_
#include "kep/Core.h"
namespace deway
{
    class Triangle
    {
    public:
        kep::Vector3 p[3];
        kep::Vector3 n;
        Triangle(kep::Vector3 _p0 = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _p1 = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _p2 = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _n = kep::Vector3(0.0f ,0.0f, 0.0f));
        ~Triangle();
    };
};
#endif //TRIANGLE_DEWAY_H_
