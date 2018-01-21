#include "NMGen.h"
#include <chrono>
using namespace deway;
#define EXEC_TIMER(o_timeElapsed, _expr) \
{\
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();\
            _expr;\
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();\
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);\
    o_timeElapsed = time_span.count();\
}


#define NUM_SAMPLES 10000
#define EXEC_TIMER_SAMPLE(o_timeElapsed0, _expr0) \
{\
        double tti0 = 0.0f; \
        for(int i = 0 ;i < NUM_SAMPLES;i++)\
        {\
            double ti0 = 0.0f;\
            EXEC_TIMER(ti0, _expr0);\
            tti0 = tti0 + ti0;\
        }\
        o_timeElapsed0 = tti0/(double)NUM_SAMPLES;\
}


NMGen::NMGen(float * _vertexData, float * _normalData, uint _numVertex, uint _volX, uint _volY, uint _volZ, kep::Vector3 _offset, bool _autoSizeVolume, float _voxelSize,float _maxSlope)
{
    float * dataV = _vertexData;
    float * dataN = _normalData;
    
    m_numTriangles = _numVertex/3;
    m_triangles = new Triangle[m_numTriangles];
    
    for(uint i = 0; i < _numVertex*3; i = i + 9)
    {
        m_triangles[i/9].p[0] = kep::Vector3( dataV[i+0], dataV[i+1], dataV[i+2]);
        m_triangles[i/9].p[1] = kep::Vector3( dataV[i+3], dataV[i+4], dataV[i+5]);
        m_triangles[i/9].p[2] = kep::Vector3( dataV[i+6], dataV[i+7], dataV[i+8]);
        m_triangles[i/9].n    = kep::Vector3( dataN[i+0], dataN[i+1], dataN[i+2]);
        m_triangles[i/9].genAABB();//generate bounding box
    }
    
    
    m_volX = _volX;
    m_volY = _volY;
    m_volZ = _volZ;
    
    
    
    m_voxelSize = _voxelSize;
    
    m_overlapVoxels = NULL;
    m_numOverlapVoxels = 0;
    m_offset = _offset;
    m_maxSlope = _maxSlope;
    m_numVoxel = m_volX * m_volY * m_volZ;
    if(_autoSizeVolume)
        autoSizeVoxelVolume();
    m_voxels = new Voxel[m_numVoxel];
    

    genSpans();
    
    voxelize();
    
}
NMGen::~NMGen()
{
    delete[] m_triangles;
    delete[] m_overlapVoxels;
    delete[] m_spans;
}

void NMGen::genSpans()
{
    m_numSpans = m_volX * m_volZ;
    m_spans = new Span[m_numSpans];
    for(uint i = 0; i<m_numSpans; i++)
        m_spans[i].gen(m_volY);
}
void NMGen::autoSizeVoxelVolume()
{
    //Find the mesh center
    float d3 = 1.0f/(((float)m_numTriangles)*3.0f);
    kep::Vector3 c;
    for(uint i = 0; i<m_numTriangles; i++)
        for(uint j = 0; j<3; j++)
        {
            c = c + m_triangles[i].p[j];
        }
        c = c * d3;
    
    float max[3]={0.0f};
    for(uint i = 0; i<m_numTriangles; i++)
        for(uint j = 0; j<3; j++)
            for(uint k = 0; k<3; k++)
            {
                float data = std::abs(m_triangles[i].p[j].data[k] - c.data[k]);
                if(data > max[k])
                    max[k] = data;
            }
    m_offset = c;
    m_volX = (uint)((max[0]) / (m_voxelSize))+1;
    m_volY = (uint)((max[1]) / (m_voxelSize))+1;
    m_volZ = (uint)((max[2]) / (m_voxelSize))+1;
    m_numVoxel = m_volX * m_volY * m_volZ; 
    
}


void NMGen::genVoxelVolume()
{
    uint iter = 0;
    for(uint y = 0; y<m_volY; y++)
    {
        uint xz = 0;
        for(uint x = 0; x<m_volX; x++)
            for(uint z = 0; z<m_volZ; z++)
            {
                m_voxels[iter] = Voxel(AABB(kep::Vector3(((float)x * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volX)/2) + m_voxelSize + m_offset.x, 
                                                    ((float)y * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volY)/2) + m_voxelSize + m_offset.y, 
                                                    ((float)z * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volZ)/2) + m_voxelSize + m_offset.z),
                                       kep::Vector3(m_voxelSize, m_voxelSize, m_voxelSize)));
                m_spans[xz].m_voxels[y] = &m_voxels[iter];
                iter++;
                xz++;
            }
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
    
    double singleExecTime = 0.0;
    EXEC_TIMER(singleExecTime,
    m_voxels[0].aabb.triTest(&m_triangles[0]);
    );
    printf("----Voxelizer info----\n");
    printf("Vox vol dimensions: %u %u %u\n", m_volX, m_volY, m_volZ);
    printf("Num vox: %u \n", m_numVoxel);
    printf("Num tri: %u \n", m_numTriangles);
    printf("Num test: %u \n", m_numVoxel*m_numTriangles);
    printf("vox-tri time: %f \n", singleExecTime);
    printf("Est worst-case vox time: %f \n", singleExecTime * m_numVoxel * m_numTriangles);
    
    double execTime = 0.0;
    EXEC_TIMER(execTime, 
    //flag overlaping voxels
    for(uint i = 0; i<m_numVoxel; i++)
    {
        for(uint j = 0; j<m_numTriangles; j++)
        {
            if(m_voxels[i].aabb.aabbTest(m_triangles[j].aabb) == 1)//MAJOR optimization
            {
                if(m_voxels[i].aabb.triTest(&m_triangles[j]) == 1)
                    if(m_voxels[i].overlaps == false && slopeCheck(&m_triangles[j]) == true)//0.9 being small slopes, 0.5 big slopes
                    {
                        m_voxels[i].overlaps = true;
                        m_numOverlapVoxels++;
                    }
            }
        }
    }
    );
    printf("Vox time: %f\n", execTime);
    
    
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




