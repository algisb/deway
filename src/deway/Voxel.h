#ifndef VOXEL_DEWAY_H_
#define VOXEL_DEWAY_H_
#include "Core.h"
namespace deway 
{
    class Voxel
    {
    public:
        kep::Vector3 c;//center
        kep::Vector3 hs;//halfsize
        bool overlaps;
        Voxel(kep::Vector3 _center = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _halfSize = kep::Vector3(0.0f, 0.0f, 0.0f));
        ~Voxel();
        
    };
};

#endif //VOXEL_DEWAY_H_
