#ifndef DEWAY_VERTEX_H_
#define DEWAY_VERTEX_H_
#include "kep/Core.h"

namespace deway 
{
    class Vertex 
    {
    public:
        kep::Vector3 pos;
        bool locked; //used in contour generation stage
        Vertex();
        ~Vertex();
    };
};
#endif
