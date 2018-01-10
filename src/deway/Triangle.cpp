#include "Triangle.h"
using namespace deway;

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
