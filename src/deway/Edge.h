#ifndef EDGE_DEWAY_H_
#define EDGE_DEWAY_H_
#include <stdlib.h>
#include "../kep/Core.h"
namespace deway 
{
    class Voxel;
    class Edge 
    {
    public:
        bool external;//to flag as external edge
        kep::Vector3 v[2];
        Voxel * nghbr[2];
        Voxel * creator;//stores memory location of the voxels which initialised this edge
        Edge();
        ~Edge();
    };
};
#endif
