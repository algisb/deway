#include "Span.h"
#include "Voxel.h"
#include <stdlib.h>
using namespace deway;
Span::Span()
{
    m_size = 0;
    m_voxels = NULL;
    for(int i = 0; i<8; i++)
        nghbr[i] = NULL;
}

Span::~Span()
{
    delete [] m_voxels;
}

void Span::gen(uint _size)
{
    m_size = _size;
    m_voxels = new Voxel*[m_size];
}

