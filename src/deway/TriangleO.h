#ifndef TRIANGLEO_H_
#define TRIANGLEO_H_
#include "typedefs.h"
#include "Core.h"

namespace deway
{
    class Vertex;
    class TriangleO 
    {
    public:
        Vertex * vertex[3];
        TriangleO * nghbr[3];//neighbour triangles
        kep::Vector3 normal;
        TriangleO(Vertex * _v0, Vertex * _v1, Vertex * _v2);
        ~TriangleO();
    };
};
#endif
