#include "Box.h"
using namespace deway;
Box::Box(kep::Vector3 _center, kep::Vector3 _halfSize)
{
    c = _center;
    hs = _halfSize;
}
Box::~Box()
{
}

