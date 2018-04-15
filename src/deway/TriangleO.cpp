#include "TriangleO.h"
#include <stdlib.h>
using namespace deway;

TriangleO::TriangleO(Vertex * _v0, Vertex * _v1, Vertex * _v2)
{
    vertex[0] = _v0;
    vertex[1] = _v1;
    vertex[2] = _v2;

}
TriangleO::~TriangleO()
{
    for(uint i = 0; i<3; i++)
        vertex[i] = NULL;
}

