#ifndef VOXEL_DEWAY_H_
#define VOXEL_DEWAY_H_
#include "AABB.h"
namespace deway 
{
    class Voxel
    {
    public:
        AABB aabb;
        bool overlaps;
        
//         union
//         {
//             Voxel * nghbr[26];
//         };
        
        Voxel(AABB _aabb = AABB());
        ~Voxel();
        
    };
};

#endif //VOXEL_DEWAY_H_
