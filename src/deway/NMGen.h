#ifndef NMGEN_H_
#define NMGEN_H_
#include "Triangle.h"
#include "Box.h"
#include "TriBoxTest.h"

namespace deway
{
    class NMGen
    {
    public:
        uint64_t m_numTriangles;
        Triangle * m_triangles;
        uint64_t m_volX;
        uint64_t m_volY;
        uint64_t m_volZ;
        
        float m_voxelSize;
        uint64_t m_numVoxel;
        Box * m_voxelVolume;
        
        uint64_t m_numOverlapVoxel;
        Box * m_overlapVoxels;
        NMGen(float * _vertexData, float * _normalData, uint64_t _numVertex, uint64_t _volX, uint64_t _volY, uint64_t _volZ, float _voxelSize);
        ~NMGen();
        void voxelize();
        void genVoxelVolume();
        
    };
};

#endif //NMGEN_H_
