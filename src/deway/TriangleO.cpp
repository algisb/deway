#include "TriangleO.h"
#include <stdlib.h>
using namespace deway;

TriangleO::TriangleO()
{
    for(uint i = 0; i<3; i++)
        vertex[i] = NULL;
}
TriangleO::~TriangleO()
{
}

