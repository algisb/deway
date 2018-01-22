#include "Voxel.h"
#include "AABB.h"
#include <stdlib.h>
using namespace deway;
Voxel::Voxel()
{
    aabb = NULL;
    overlaps = false;
}
Voxel::~Voxel()
{
    delete aabb;
}
void Voxel::genAABB(AABB _aabb)
{
    aabb = new AABB();
    *aabb = _aabb;
}

