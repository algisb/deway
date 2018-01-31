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
        
        void expand();
        void create();
        void merge();
    };
};

#endif //REGGEN_H_
