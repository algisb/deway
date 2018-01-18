#include "AABB.h"
#include "Triangle.h"
using namespace deway;




AABB::AABB(kep::Vector3 _center, kep::Vector3 _halfSize)
{
    c = _center;
    hs = _halfSize;
}
AABB::~AABB()
{
}
