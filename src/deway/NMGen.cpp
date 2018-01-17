#include "NMGen.h"
using namespace deway;
NMGen::NMGen(float * _vertexData, float * _normalData, uint _numVertex,
    uint _volX, uint _volY, uint _volZ, float _voxelSize,float _maxSlope, kep::Vector3 _offset)
{
    float * dataV = _vertexData;
    float * dataN = _normalData;
    
    m_numTriangles = _numVertex/3;
    m_triangles = new Triangle[m_numTriangles];
    
    for(uint i = 0; i < _numVertex*3; i = i + 9)
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
    m_voxels = new Voxel[m_numVoxel];
    m_overlapVoxels = NULL;
    m_numOverlapVoxels = 0;
    m_offset = _offset;
    m_maxSlope = _maxSlope;
    
    voxelize();
    
}
NMGen::~NMGen()
{
    delete[] m_triangles;
}
void NMGen::genVoxelVolume()
{
    uint iter = 0;
    for(int x = 0; x<m_volX; x++)
        for(int y = 0; y<m_volY; y++)
            for(int z = 0; z<m_volZ; z++)
            {
                m_voxels[iter] = Voxel(kep::Vector3(((float)x * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volX)/2) + m_voxelSize + m_offset.x, 
                                                    ((float)y * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volY)/2) + m_voxelSize + m_offset.y, 
                                                    ((float)z * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volZ)/2) + m_voxelSize + m_offset.z),
                                       kep::Vector3(m_voxelSize, m_voxelSize, m_voxelSize));
                iter++;
            }
}
bool NMGen::slopeCheck(Triangle * _t)
{
    kep::Vector3 up(0.0f, 1.0f, 0.0f);
    float slope = kep::dot(up, _t->n);
    if(slope > m_maxSlope || slope < -m_maxSlope)
        return true;
    else
        return false;
}
void NMGen::voxelize()
{
    genVoxelVolume();
    
    //flag overlaping voxels
    for(uint i = 0; i<m_numVoxel; i++)
    {
        for(uint j = 0; j<m_numTriangles; j++)
        {
            if(triBoxTest(m_triangles[j], m_voxels[i]) == 1)
            {
                if(m_voxels[i].overlaps == false && slopeCheck(&m_triangles[j]) == true)//0.9 being small slopes 0.5 being big slopes
                {
                    m_voxels[i].overlaps = true;
                    m_numOverlapVoxels++;
                }
            }
        }
    }
    
    //get a list of refs to the overlaping voxels
    uint iter = 0;
    m_overlapVoxels = new Voxel*[m_numOverlapVoxels];
    for(uint i = 0; i<m_numVoxel; i++)
    {
        if(m_voxels[i].overlaps == true)
        {
            m_overlapVoxels[iter] = &m_voxels[i];
            iter++;
        }
    }
    
}




