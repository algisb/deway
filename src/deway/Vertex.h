#ifndef DEWAY_VERTEX_H_
#define DEWAY_VERTEX_H_
#include "kep/Core.h"

namespace deway 
{
    class Vertex 
    {
    public:
        kep::Vector3 pos;
        bool locked;
        Vertex();
        ~Vertex();
    };
};
#endif
