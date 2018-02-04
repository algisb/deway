#include "Voxel.h"
#include "AABB.h"
#include <stdlib.h>
using namespace deway;
Voxel::Voxel()
{
    aabb = NULL;
    //overlaps = false;
    traversable = false;
    blacklisted = false;
    for(uint i = 0; i<8; i++)
        nghbr[i] = NULL;
    edge = false;
    dist = 0.0f;
    reg = NULL;
    regCounter = 0;
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

