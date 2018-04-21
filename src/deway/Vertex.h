#ifndef DEWAY_VERTEX_H_
#define DEWAY_VERTEX_H_
#include "kep/Core.h"
#include <vector>
namespace deway 
{
    class TriangleO;
    class Vertex
    {
    public:
        kep::Vector3 pos;
        bool locked; //used in contour generation stage
        std::vector<TriangleO*> nghbr;
        Vertex * parent;
        float G_cost;
        float H_cost;
        float F_cost;
        Vertex();
        ~Vertex();
    };
};
#endif
