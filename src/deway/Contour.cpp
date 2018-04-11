#include "Contour.h"
using namespace deway;

Contour::Contour()
{
}

Contour::~Contour()
{
    for(uint i = 0; i<m_verts.size(); i++)
        delete m_verts[i];
}




