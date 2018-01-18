#include "Voxel.h"
using namespace deway;
Voxel::Voxel(AABB _aabb)
{
    aabb = _aabb;
    overlaps = false;
}
Voxel::~Voxel()
{
}

