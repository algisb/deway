#ifndef DEWAY_REGION_H_
#define DEWAY_REGION_H_
#include "typedefs.h"
#include <vector>
namespace deway
{
    class Voxel;
    class Region
    {
    public:
        std::vector<Voxel*> m_voxels;
        Region();
        ~Region();
        int addVoxelRef(Voxel * _v);
        
        
    };
};


#endif //DEWAY_REGION_H_
