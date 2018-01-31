#include "RegGen.h"

using namespace deway;

RegGen::RegGen(Voxel ** _travVoxels, uint _numTravVoxels, float _maxEdgeDist)
{
    m_travVoxelsRef = _travVoxels;
    m_numTravVoxels  = _numTravVoxels;
    m_maxEdgeDist = _maxEdgeDist;
    
    expand();
    create();
    merge();
}
RegGen::~RegGen()
{
    for(uint i = 0; i<m_regions.size(); i++)
        delete m_regions[i];
}

void RegGen::expand()
{
    for(uint i = 0; i < m_regions.size(); i++)
    {
        
    }
}

void RegGen::create()
{
    
}

void RegGen::merge()
{
    
}
