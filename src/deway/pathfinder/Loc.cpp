#include "Loc.h"
using namespace deway;

Loc::Loc(TriangleO* _tri, kep::Vector3 _pos)
{
    tri = _tri;
    pos = _pos;
}
Loc::~Loc()
{
}

