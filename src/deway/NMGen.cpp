#include "NMGen.h"
using namespace deway;
NMGen::NMGen(float * _vertexData, float * _normalData, uint64_t _numVertex,
    uint64_t _volX, uint64_t _volY, uint64_t _volZ, float _voxelSize)
{
    float * dataV = _vertexData;
    float * dataN = _normalData;
    
    m_numTriangles = _numVertex/3;
    m_triangles = new Triangle[m_numTriangles];
    
    for(uint64_t i = 0; i < _numVertex*3; i = i + 9)
    {
        m_triangles[i/9] = Triangle(kep::Vector3( dataV[i+0], dataV[i+1], dataV[i+2]),
                            kep::Vector3( dataV[i+3], dataV[i+4], dataV[i+5]),
                            kep::Vector3( dataV[i+6], dataV[i+7], dataV[i+8]),
                            kep::Vector3( dataN[i+0], dataN[i+1], dataN[i+2])
                                      );
    }
    //printf("%f \n", *dataV);
//     m_triangles[0].p[0].dump();
//     m_triangles[0].p[1].dump();
//     m_triangles[0].p[2].dump();
    m_volX = _volX;
    m_volY = _volY;
    m_volZ = _volZ;
    m_numVoxel = m_volX * m_volY * m_volZ; 
    m_voxelSize = _voxelSize;
    m_voxels = new Box[m_numVoxel];
    m_overlapVoxels = new Box[m_numVoxel];
    m_numOverlapVoxel = 0;
    voxelize();
    
}
NMGen::~NMGen()
{
    delete[] m_triangles;
}
void NMGen::voxelize()
{
    genVoxelVolume();
    for(uint64_t i = 0; i<m_numVoxel; i++)
    {
        for(uint64_t j = 0; j<m_numTriangles; j++)
        {
            if(triBoxTest(m_triangles[j], m_voxels[i]) == 1)
            {
                m_overlapVoxels[m_numOverlapVoxel].c = m_voxels[i].c;
                m_overlapVoxels[m_numOverlapVoxel].hs = m_voxels[i].hs;
                m_numOverlapVoxel++;
            }
        }
    }
    
}
void NMGen::genVoxelVolume()
{
    uint64_t iter = 0;
    for(int x = 0; x<m_volX; x++)
        for(int y = 0; y<m_volY; y++)
            for(int z = 0; z<m_volZ; z++)
            {
                m_voxels[iter] = Box(kep::Vector3(((float)x * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volX)/2) + m_voxelSize, ((float)y * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volX)/2) + m_voxelSize, ((float)z * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volX)/2) + m_voxelSize) , kep::Vector3(m_voxelSize, m_voxelSize, m_voxelSize));
                iter++;
            }
}



