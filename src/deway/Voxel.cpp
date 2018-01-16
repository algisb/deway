#include "Voxel.h"
using namespace deway;
Voxel::Voxel(kep::Vector3 _center, kep::Vector3 _halfSize)
{
    c = _center;
    hs = _halfSize;
    overlaps = false;
}
Voxel::~Voxel()
{
}

