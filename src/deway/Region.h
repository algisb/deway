#ifndef DEWAY_REGION_H_
#define DEWAY_REGION_H_
#include "typedefs.h"

namespace deway
{
    class Voxel;
    class Region
    {
    public:
        uint m_numVoxels;
        uint m_numMaxVoxels;
        Voxel ** m_voxels;
        
        Region();
        ~Region();
        void gen(uint _numMaxVoxels);
        int addVoxelRef(Voxel * _v);
        
        
    };
};


#endif //DEWAY_REGION_H_
