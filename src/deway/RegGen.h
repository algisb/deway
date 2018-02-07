#ifndef REGGEN_H_
#define REGGEN_H_
#include "Region.h"
#include <vector>

namespace deway
{
    class RegGen
    {
    public:
        std::vector<Region*> m_regions;//regions
        
        Voxel ** m_travVoxelsRef;
        uint m_numTravVoxels;
        float m_maxEdgeDist;
        
        
        RegGen(Voxel ** _travVoxels, uint _numTravVoxels, float _maxEdgeDist);
        ~RegGen();
        
        int sortRegs(std::vector<Region*> * _regions);
        int transferVoxels(Region * _from, Region * _to);
        
        void create();
        void merge();
        void countBoardRegs(std::vector<std::vector<Region*>> & _vvr, Voxel * _n);
        void filterWrappingCorners();
        
        
    };
};

#endif //REGGEN_H_
