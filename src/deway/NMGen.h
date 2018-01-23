#ifndef NMGEN_H_
#define NMGEN_H_
#include "Triangle.h"
#include "Span.h"
#include "Voxel.h"
#include "typedefs.h"

namespace deway
{ 

    class NMGen
    {
    public:
        uint m_numTriangles;
        Triangle * m_triangles;
        uint m_volX;
        uint m_volY;
        uint m_volZ;
        
        float m_voxelSize;//this is a HALF-LENGTH
        float m_maxSlope;
        uint m_numVoxel;
        Voxel * m_voxels;
        
        uint m_numTravVoxels;
        Voxel ** m_travVoxels;//array of poiters holding referances
        kep::Vector3 m_offset; //voxel volume offset from world origin
        
        uint m_numSpans;
        Span * m_spans;
        
        float m_agentHeight;
        
        
        NMGen(float * _vertexData, float * _normalData, uint _numVertex, uint _volX, uint _volY, uint _volZ, kep::Vector3 _offset, bool _autoSizeVolume, float _voxelSize,float _maxSlope, float _agentHeight);
        ~NMGen();
        
        void genSpans();
        void autoSizeVoxelVolume();
        void genVoxelVolume();
        bool slopeCheck(Triangle * _t);
        void heightTests();
        void getOverlapingRefs();
        
        void voxelize();
    };
};

#endif //NMGEN_H_
