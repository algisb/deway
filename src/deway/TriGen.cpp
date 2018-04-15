#include "TriGen.h"
#include "TriangleO.h"
#include "Contour.h"

using namespace deway;
TriGen::TriGen(std::vector<Contour*> * _contours)
{
    m_contours_ref = _contours;
    genTriangles();
}
TriGen::~TriGen()
{
    for(uint i = 0; i<m_navMesh.size(); i++)
        delete m_navMesh[i];
}
void TriGen::genTriangles()
{
    for(uint i = 0; i<m_contours_ref->size(); i++)
        for(uint j = 0; j<(*m_contours_ref)[i]->m_reducedVerts.size(); j++)
        {
            
        }
}

