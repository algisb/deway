#include "NMGen.h"
#include <chrono>
#include "AABB.h"
#include "RegGen.h"
#include "ContGen.h"
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


NMGen::NMGen(float * _vertexData, float * _normalData, uint _numVertex, uint _volX, uint _volY, uint _volZ, kep::Vector3 _offset, bool _autoSizeVolume, float _voxelSize,float _maxSlope, float _agentHeight, float _maxStepHeight)
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
    
    m_travVoxels = NULL;
    m_numTravVoxels = 0;
    m_offset = _offset;
    m_maxSlope = _maxSlope;
    m_numVoxels = m_volX * m_volY * m_volZ;
    if(_autoSizeVolume)
        autoSizeVoxelVolume();
    m_voxels = new Voxel[m_numVoxels];
    m_agentHeight = _agentHeight;
    m_maxStepHeight = _maxStepHeight;
    m_numSpans = 0;
    m_spans = NULL;
    
    m_numEdgeVoxels = 0;
    m_edgeVoxels = NULL;
    m_maxEdgeDist = 0.0f; //maximum value inside the distance field
    m_regGen = NULL;
    m_contGen = NULL;

    
    
    genSpans();
    genVoxelVolume();
    
    
    voxelize();
    
    getSpanNeighbours();
    getVoxelNeighbours();
    
    getTraversableVoxels();//fills m_travVoxels
    getEdgeVoxels();//fills m_edgeVoxels
    
    calcEdgeDistances();
    genRegions();
    genContours();
    
    
}
NMGen::~NMGen()
{
    delete[] m_triangles;
    delete[] m_travVoxels;
    delete[] m_spans;
    delete m_regGen;
    delete m_contGen;
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
//     m_volX = (uint)((max[0]) / (m_voxelSize))+1;
//     m_volY = (uint)((max[1]) / (m_voxelSize))+1;
//     m_volZ = (uint)((max[2]) / (m_voxelSize))+1;
    
    m_volX = 0;
    
    while(1)
    {
        if( (((m_volX * (m_voxelSize*2))/2)) > max[0] )
            break;
        m_volX++;
    }
    m_volY = 0;
    while(1)
    {
        if( (((m_volY *(m_voxelSize*2))/2)) > max[1] )
            break;
        m_volY++;
    }
    
    m_volZ = 0;
    while(1)
    {
        if( (((m_volZ * (m_voxelSize*2))/2)) > max[2] )
            break;
        m_volZ++;
    }
    m_numVoxels = m_volX * m_volY * m_volZ; 
    
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
                m_voxels[iter].genAABB(AABB(kep::Vector3(((float)x * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volX)/2) + m_voxelSize + m_offset.x, 
                                                    ((float)y * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volY)/2) + m_voxelSize + m_offset.y, 
                                                    ((float)z * m_voxelSize * 2) - ((m_voxelSize * 2 *m_volZ)/2) + m_voxelSize + m_offset.z),
                                       kep::Vector3(m_voxelSize, m_voxelSize, m_voxelSize)));
                
                m_spans[xz].m_voxels[y] = &m_voxels[iter];
                iter++;
                xz++;
            }
    }
            
}


void NMGen::heightTests()
{
    for(uint i = 0; i<m_numSpans; i++)
    {
            uint nv = 0;
            Voxel ** v = new Voxel*[m_spans[i].m_size];
            
            //extract overlaping and traversable voxels from the span
            for(uint j = 0; j<m_spans[i].m_size; j++)
            {
                if(m_spans[i].m_voxels[j]->overlaps || m_spans[i].m_voxels[j]->traversable)
                {
                    v[nv] = m_spans[i].m_voxels[j];
                    nv++;
                }
            }
            
            
            //perform height checks between the voxels
            if(nv > 1)//span cointains more than 1 overlaping/traversable voxel
            {
                for(uint j = 0; j<nv; j++) //
                {
                    if(!v[j]->traversable)//only traversable voxels need height check
                        continue;
                        
                    uint j2 = j+1;
                    if(j2 > nv-1)//the last voxel does not get tested
                        break;
                    
                    float max0 = v[j]->aabb->c.y + v[j]->aabb->hs.y;
                    float max1 = v[j2]->aabb->c.y + v[j]->aabb->hs.y;
                    
                    float d = std::abs(max1 - max0);
                    if(d < m_agentHeight)
                    {
                         v[j]->traversable = false;
                         m_numTravVoxels--;
                    }
                }
            }
            
            
            delete [] v;
    }
}

void NMGen::getTraversableVoxels()
{
    //get a list of refs to the overlaping voxels
    if(m_numTravVoxels > 0)
    {
        uint iter = 0;
        m_travVoxels = new Voxel*[m_numTravVoxels];
        for(uint i = 0; i<m_numVoxels; i++)
        {
            if(m_voxels[i].traversable == true)
            {
                m_travVoxels[iter] = &m_voxels[i];
                iter++;
            }
        }
    }
}

void NMGen::voxelize()
{
    double singleExecTime = 0.0;
    EXEC_TIMER(singleExecTime,
    m_voxels[0].aabb->triTest(&m_triangles[0]);
    );
    printf("----Voxelizer info----\n");
    printf("Vox vol dimensions: %u %u %u\n", m_volX, m_volY, m_volZ);
    printf("Num vox: %u \n", m_numVoxels);
    printf("Num tri: %u \n", m_numTriangles);
    printf("Num test: %u \n", m_numVoxels*m_numTriangles);
    printf("vox-tri time: %f \n", singleExecTime);
    printf("Est worst-case vox time: %f \n", singleExecTime * m_numVoxels * m_numTriangles);
    
    double execTime = 0.0;
    EXEC_TIMER(execTime,
    
    for(uint i = 0; i<m_numSpans; i++)
    {
        for(uint j = 0; j<m_numTriangles; j++)
        {
            if(m_triangles[j].aabb->spanTest(&m_spans[i]) == 1)//MAJOR optimization
            {
                for(uint k = 0; k<m_spans[i].m_size; k++)
                {
                    if(m_spans[i].m_voxels[k]->aabb->triTest(&m_triangles[j]) == 1)
                    {
                        m_spans[i].m_voxels[k]->overlaps = true;
                        if(m_spans[i].m_voxels[k]->traversable == false && slopeCheck(&m_triangles[j]) == true && !m_spans[i].m_voxels[k]->blacklisted)//0.9 being small slopes, 0.5 big slopes
                        {
                            m_spans[i].m_voxels[k]->traversable = true;
                            m_numTravVoxels++;
                        }
                        else
                        {
                            m_spans[i].m_voxels[k]->blacklisted = true;
                        }
                    }
                }
            }
        }
    }
    
    
    );
    printf("Vox time: %f\n", execTime);
    heightTests();
}

void NMGen::getSpanNeighbours()
{
    for(uint x = 0; x < m_volX; x++)
        for(uint y = 0; y < m_volZ; y++)
        {
            Span * c = getSpan(x,y);
            c->nghbr[0] = getSpan(x,    y+1);
            c->nghbr[1] = getSpan(x+1,  y+1);
            c->nghbr[2] = getSpan(x+1,  y);
            c->nghbr[3] = getSpan(x+1,  y-1);
            c->nghbr[4] = getSpan(x,    y-1);
            c->nghbr[5] = getSpan(x-1,  y-1);
            c->nghbr[6] = getSpan(x-1,  y);
            c->nghbr[7] = getSpan(x-1,  y+1);
        }

//     Span * s = getSpan(m_volX-1, m_volZ-1);
//     for(uint i = 0; i<s->m_size; i++)
//     {
//         //s->m_voxels[i]->aabb->c.dump();
//         s->m_voxels[i]->overlaps = true;
//         s->m_voxels[i]->traversable = true;
//         m_numTravVoxels++;
//     }
//     
//     for(uint j = 0; j<8; j++)
//     {
//         if(s->nghbr[j] != NULL)
//         {
//             for(uint i = 0; i<s->nghbr[j]->m_size; i++)
//             {
//                 s->m_voxels[i]->overlaps = true;
//                 s->nghbr[j]->m_voxels[i]->traversable = true;
//                 m_numTravVoxels++;
//             }
//         }
//     }
}


bool NMGen::stepCheck(deway::Voxel* _v0, deway::Voxel* _v1)
{
    float step =  std::abs(_v0->aabb->c.y - _v1->aabb->c.y);
    if(step > m_maxStepHeight)
        return false;
    else
        return true;
}

void NMGen::getVoxelNeighbours()
{
    for(uint i = 0; i<m_numSpans; i++)
    {
        Span * span = &m_spans[i];
        for(uint j = 0; j< span->m_size; j++)
        {
            Voxel * voxel = span->m_voxels[j];
            
            
            if(voxel->traversable)//only care about traversable voxels
                for(uint k = 0; k<8; k++)
                {
                    Span * spanN = span->nghbr[k]; //neighbour span
                    if(spanN != NULL)//neighbour span exists
                    {
                        for(uint l = 0; l< spanN->m_size; l++)
                        {
                            Voxel * voxelN = spanN->m_voxels[l]; //potencial neighbour voxel
                            if(voxelN->traversable && stepCheck(voxel, voxelN))
                            {
                                voxel->nghbr[k] = voxelN;
                            }
                            
                        }
                        
                    }
                }
            
            
            
            
            
        }
        
    }
}

void NMGen::getEdgeVoxels()
{
    for(uint i = 0; i<m_numVoxels; i++)
        if(m_voxels[i].traversable == true)
            for(uint j = 0; j<8; j++)
                if(m_voxels[i].nghbr[j] == NULL)
                {
                    m_voxels[i].edge = true;
                    m_numEdgeVoxels++;
                    break;
                }
                
    m_edgeVoxels = new Voxel*[m_numEdgeVoxels];
    
    uint iter = 0;
    for(uint i = 0; i<m_numVoxels; i++)
        if(m_voxels[i].edge)
        {
            m_edgeVoxels[iter] = &m_voxels[i];
            iter++;
        }
}

void NMGen::calcEdgeDistances()
{
    //calculates edge distance for each voxel
    if(m_numTravVoxels > 0 && m_numEdgeVoxels > 0)
        for(uint i = 0; i<m_numTravVoxels; i++)
        {
            if(m_travVoxels[i]->edge)
            {
                m_travVoxels[i]->dist = 0.0f;
                continue;
            }
            m_travVoxels[i]->dist = (m_travVoxels[i]->aabb->c - m_edgeVoxels[0]->aabb->c).magnitude();
            
            for(uint j = 0; j<m_numEdgeVoxels; j++)
            {
                float dist = (m_travVoxels[i]->aabb->c - m_edgeVoxels[j]->aabb->c).magnitude();
                if(dist < m_travVoxels[i]->dist)
                    m_travVoxels[i]->dist = dist;
            }
        }
    
    //finds max edge distance
    for(uint i = 0; i<m_numTravVoxels; i++)
        if(m_travVoxels[i]->dist > m_maxEdgeDist)
            m_maxEdgeDist = m_travVoxels[i]->dist;
}

void NMGen::genRegions()
{
    m_regGen = new RegGen(m_travVoxels, m_numTravVoxels, m_maxEdgeDist);
    printf("m_maxEdgeDist: %f \n", m_maxEdgeDist);
}

void NMGen::genContours()
{
    m_contGen = new ContGen(&m_regGen->m_regions);
}










