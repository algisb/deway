#include "RegGen.h"
#include <stdio.h>
#include <stdlib.h>
#include "Voxel.h"

using namespace deway;

RegGen::RegGen(Voxel ** _travVoxels, uint _numTravVoxels, float _maxEdgeDist)
{
    m_travVoxelsRef = _travVoxels;
    m_numTravVoxels  = _numTravVoxels;
    m_maxEdgeDist = _maxEdgeDist;
    

    create();
    merge();
    
}
RegGen::~RegGen()
{
    for(uint i = 0; i<m_regions.size(); i++)
        delete m_regions[i];
}


int RegGen::sortRegs(std::vector<Region *> * _regions)
{
    if(_regions->size() < 2)
        return 0;
    
    while(1)
    {
        bool done = true;
        for(uint i = 0 ; i<_regions->size()-1; i++)
        {
            Region * r0 = (*_regions)[i];
            Region * r1 = (*_regions)[i+1];
            if(r0->m_voxels.size() > r1->m_voxels.size())
            {
                (*_regions)[i+1] = r0;
                (*_regions)[i] = r1;
                done = false;
            }
        }
        if(done)
            break;
    }
    return 0;
}
int RegGen::transferVoxels(Region* _from, Region* _to)
{
    for(uint i = 0; i < _from->m_voxels.size(); i++)
            _to->addVoxelRef(_from->m_voxels[i]);
    return 0;
}


void RegGen::create()
{
    //CONFIGS
    float ccD = 10.0f;//the smaller the number the harder it is for new regions to form
    float lastLevel = 0.0f;//4.0f;
    float raiseLevel = 0.01f;
    ///////////////////////////////////
    
    
    float w = m_maxEdgeDist;
    float createCount = (uint)((m_maxEdgeDist/raiseLevel)/ccD);//Number of expansions the voxel has to survive to seed a new region
    for(;;)
    {
        w = w - raiseLevel;
        float w0 = w;
        if(w0 < lastLevel)
            w0 = lastLevel;
        ////////////////////////////////////////////////////////////////
        for(uint i = 0; i < m_numTravVoxels; i++)
            if(m_travVoxelsRef[i]->dist >= w0 && m_travVoxelsRef[i]->reg == NULL)//at this water level and dont belong to a region yet
            {
                
                //Merge into an existing region
                for(uint j = 0; j < 8; j++)//go through each neighbour
                    if(m_travVoxelsRef[i]->nghbr[j] != NULL)//neighbour isnt empty
                    {
                        if(m_travVoxelsRef[i]->nghbr[j]->reg != NULL)//neighbour is part of a region
                        {
                            m_travVoxelsRef[i]->reg = m_travVoxelsRef[i]->nghbr[j]->reg;
                            break;
                        }
                    }
                
                if(m_travVoxelsRef[i]->reg != NULL)
                {
                    m_travVoxelsRef[i]->reg->addVoxelRef(m_travVoxelsRef[i]);
                }
                else
                {
                    if(m_travVoxelsRef[i]->regCounter < createCount)
                    {
                        m_travVoxelsRef[i]->regCounter++;
                    }
                    else
                    {
                        Region * reg = new Region();
                        m_travVoxelsRef[i]->reg = reg;
                        reg->addVoxelRef(m_travVoxelsRef[i]);
                        m_regions.push_back(reg);
                    }
                }
                
            }
        /////////////////////////////////////////////////////////////////
        if(w<lastLevel)
            break;
    }
}


void RegGen::merge()
{
    //CONFIGS
    uint minRegionSize = 100;//CONFIGURATION PARAMETER
    //////////////////////
    
    //collect small regions
    std::vector<Region*> smallRegions;
    for(uint i = 0; i < m_regions.size(); i++)
        if(m_regions[i]->m_voxels.size() < minRegionSize)
            smallRegions.push_back(m_regions[i]);
        
    sortRegs(&smallRegions);
//     for(uint i = 0; i < smallRegions.size(); i++)
//     {
//         printf("reg size: %zu \n", smallRegions[i]->m_voxels.size());
//     }
    
    //respective regions to which the small regions will be merged
    Region ** mergeRegions = new Region*[smallRegions.size()];
    for(uint i = 0; i < smallRegions.size(); i++)
        mergeRegions[i] = NULL;
        
    
    //find merge regions
    for(uint i = 0; i < smallRegions.size(); i++)
    {
        bool mergeRegFound = false;
        for(uint j = 0; j < smallRegions[i]->m_voxels.size(); j++)
        {
            Voxel * v = smallRegions[i]->m_voxels[j];
            for(uint k = 0; k<8; k++)
                if(v->nghbr[k] != NULL)
                    if(v->nghbr[k]->reg != NULL)
                        if(v->nghbr[k]->reg->m_voxels.size() > minRegionSize)
                        {
                            mergeRegions[i] = v->nghbr[k]->reg;
                            mergeRegFound = true;
                            break;
                        }
                
            if(mergeRegFound)
            break;
        }
    }
    
    
    //add voxels from small regions to the larger ones
    for(uint i = 0; i < smallRegions.size(); i++)
        if(mergeRegions[i] != NULL)
            for(uint j = 0; j < smallRegions[i]->m_voxels.size(); j++)
                mergeRegions[i]->addVoxelRef(smallRegions[i]->m_voxels[j]);
    
    //delete the small regions
    //printf("num small: %u \n", smallRegions.size());
    for(uint i = 0; i < smallRegions.size(); i++)
        for(uint j = 0; j< m_regions.size(); j++)
            if(smallRegions[i] == m_regions[j])
            {
                //printf("eq\n");
                delete m_regions[j];
                m_regions.erase(m_regions.begin()+j);
            }
    
    
    delete [] mergeRegions;
}
