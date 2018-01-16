#ifndef NMGEN_H_
#define NMGEN_H_
#include "Triangle.h"
#include "Voxel.h"
#include "TriBoxTest.h"

namespace deway
{ 
    typedef uint32_t uint;
    class NMGen
    {
    public:
        uint m_numTriangles;
        Triangle * m_triangles;
        uint m_volX;
        uint m_volY;
        uint m_volZ;
        
        float m_voxelSize;
        uint m_numVoxel;
        Voxel * m_voxels;
        
        uint m_numOverlapVoxel;
        Voxel ** m_overlapVoxels;//array of poiters holding referances
        NMGen(float * _vertexData, float * _normalData, uint _numVertex, uint _volX, uint _volY, uint _volZ, float _voxelSize);
        ~NMGen();
        void voxelize();
        void genVoxelVolume();
        
    };
};

#endif //NMGEN_H_
