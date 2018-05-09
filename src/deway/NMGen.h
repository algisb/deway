#ifndef NMGEN_H_
#define NMGEN_H_
#include "Triangle.h"
#include "Span.h"
#include "Voxel.h"
#include "typedefs.h"



namespace deway
{ 
    class ContGen;
    class RegGen;
    class TriGen;
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
        uint m_numVoxels;
        Voxel * m_voxels;
        
        uint m_numTravVoxels;
        Voxel ** m_travVoxels;//array of poiters holding referances
        kep::Vector3 m_offset; //voxel volume offset from world origin
        
        uint m_numSpans;
        Span * m_spans;
        
        float m_agentHeight;
        float m_maxStepHeight;
        
        /////////
        uint m_numEdgeVoxels;
        Voxel ** m_edgeVoxels;
        float m_maxEdgeDist;
        
        RegGen * m_regGen; //region generator
        ContGen * m_contGen; //contour generator
        TriGen * m_triGen; //polygon generator
        
        NMGen(float * _vertexData, float * _normalData, uint _numVertex, uint _volX, uint _volY, uint _volZ, kep::Vector3 _offset, bool _autoSizeVolume, float _voxelSize,float _maxSlope, float _agentHeight, float _maxStepHeight);
        ~NMGen();
        
        void genSpans();
        void autoSizeVoxelVolume();
        void genVoxelVolume();
        inline bool slopeCheck(Triangle * _t)
        {
            kep::Vector3 up(0.0f, 1.0f, 0.0f);
            float slope = kep::dot(up, _t->n);
            if(slope > m_maxSlope /*|| slope < -m_maxSlope*/)
                return true;
            else
                return false;
        }
        void heightTests();
        void getTraversableVoxels();
        
        void voxelize();
        
        
        inline Span * getSpan(uint _x, uint _y)
        {
            uint X = m_volX;//num rows
            uint Y = m_volZ;//number of elements inside the row
            
            if((_x < 0 || _x > (X-1))  ||  (_y < 0 || _y > (Y-1)))
                return NULL;
            else
            {
                uint i = _x*Y + _y;
                return &m_spans[i];
            }
        }
        void getSpanNeighbours();
        bool stepCheck(Voxel * _v0, Voxel * _v1);

        void getVoxelNeighbours();
        void getEdgeVoxels();
        void calcEdgeDistances();
        void genRegions();
        void genContours();
        void genPolygons();
    };
};

#endif //NMGEN_H_
