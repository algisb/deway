#include "Region.h"
#include "Voxel.h"
#include <stdlib.h>
#include <stdio.h>
using namespace deway;
Region::Region()
{
    m_numVoxels = 0;
    m_numMaxVoxels = 0;
    m_voxels = NULL;
}

Region::~Region()
{
    delete [] m_voxels;
}

void Region::gen(uint _numMaxVoxels)
{
    m_numMaxVoxels = _numMaxVoxels;
    m_voxels = new Voxel*[m_numMaxVoxels];
}

int Region::addVoxelRef(Voxel * _v)
{
    m_voxels[m_numVoxels] = _v;
    m_numVoxels++;
    return 0;
}

