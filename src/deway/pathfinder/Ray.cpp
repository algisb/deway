#include "Ray.h"
using namespace deway;

Ray::Ray(kep::Vector3 _source, kep::Vector3 _direction)
{
    s = _source;
    d = _direction;
}

Ray::~Ray()
{
}
