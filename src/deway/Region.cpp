#include "Region.h"
#include "Voxel.h"
#include <stdlib.h>
#include <stdio.h>
using namespace deway;
Region::Region()
{
}

Region::~Region()
{
}

int Region::addVoxelRef(Voxel * _v)
{
    m_voxels.push_back(_v);
    return 0;
}
uint Region::size()
{
    return m_voxels.size();
}


