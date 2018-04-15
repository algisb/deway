#ifndef TRIANGLEO_H_
#define TRIANGLEO_H_
#include "typedefs.h"

namespace deway
{
    class Vertex;
    class TriangleO 
    {
    public:
        Vertex * vertex[3];
        TriangleO();
        ~TriangleO();
    };
};
#endif
