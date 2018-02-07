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
    filterWrappingCorners();
    
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
    {
            _from->m_voxels[i]->reg = _to;
            _to->addVoxelRef(_from->m_voxels[i]);
    }
    return 0;
}


void RegGen::create()
{
    //CONFIGS
    float ccD = 10.0f;//10.0f; best value //the smaller the number the harder it is for new regions to form
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
                            if(m_travVoxelsRef[i]->reg == NULL)
                                m_travVoxelsRef[i]->reg = m_travVoxelsRef[i]->nghbr[j]->reg;
                            else
                                if(m_travVoxelsRef[i]->reg->m_voxels.size() < m_travVoxelsRef[i]->nghbr[j]->reg->m_voxels.size())
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
    uint minRegionSize = 200;//CONFIGURATION PARAMETER
    //////////////////////

    
    //find merge regions
    uint i = 0;
    while(i < m_regions.size())//for each region
    {
        
        
        //printf("iter: %u \n", i);
        
        
        if(m_regions[i]->size() < minRegionSize)//region smaller than specified, meaning needs to be merged or deleted
        {
            //attempt merging the region
            bool merged = false;
            Region * r = m_regions[i];
            for(uint j = 0; j<m_regions[i]->size(); j++)//for each voxel in the region
            {
                Voxel * v = r->m_voxels[j];
                for(uint k = 0; k<8; k++)//for each neighbour voxel
                {
                    Voxel * n = v->nghbr[k];
                    if( n != NULL)
                        if(n->reg != NULL)
                            if(n->reg != v->reg)
                            {
                                transferVoxels(v->reg, n->reg);
                                merged = true;
                                break;
                            }
                }
                if(merged)
                    break;
            }
            //delete the region
            delete m_regions[i];
            m_regions.erase(m_regions.begin() + i);
            continue;
        }
        i++;
    }
    
}


void RegGen::countBoardRegs(std::vector<std::vector<Region*>> & _vvr, Voxel * _n)
{
    bool ext = false;
    for(uint i = 0; i<_vvr.size(); i++)
    {
        if(_vvr[i].size() > 0)
            if(_vvr[i][0] == _n->reg)
            {
                _vvr[i].push_back(_n->reg);
                ext = true;
            }
    }
    
    if(!ext)
    {
        std::vector<Region*> vr;
        vr.push_back(_n->reg);
        _vvr.push_back(vr);
    }
}

void RegGen::filterWrappingCorners()
{
    uint numFilter = 2; //CONFIG PARAMETER
    ///////////////////////////////////////
    //scan all voxels and assgin then to region which it has more connections to
    for(uint nk = 0; nk <numFilter; nk ++)
        for(uint i = 0; i<m_regions.size(); i++)
        {
            Region * r = m_regions[i];
            for(uint j = 0; j < r->size(); j++)
            {
                Voxel * v = r->m_voxels[j];
                std::vector<std::vector<Region*>> vvr;
                for(uint k = 0; k<8; k++)
                {
                    Voxel * n = v->nghbr[k];
                    if(n != NULL)
                        countBoardRegs(vvr, n);
                }
                std::vector<Region*> * pvr = NULL;
                for(uint k = 0; k<vvr.size(); k++)
                {
                    if(pvr != NULL)
                    {
                        if(vvr[k].size() > pvr->size())
                            pvr = &vvr[k];
                    }
                    else
                    {
                        pvr = &vvr[k];
                    }
                }
                
                Region * nr = (*pvr)[0];
                if(v->reg != nr)//move the voxel to the new region
                {
                    v->reg = nr;
                    r->m_voxels.erase(r->m_voxels.begin() + j);
                    nr->m_voxels.push_back(v);
                    j--;
                }
                
                
            }
        }
    
    
}




