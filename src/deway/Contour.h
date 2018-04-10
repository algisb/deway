#ifndef CONTOUR_DEWAY_H_
#define CONTOUR_DEWAY_H_
#include <stdlib.h>
#include <vector>
#include "Vertex.h"

namespace deway 
{
    class Edge;
    class Contour 
    {
    public:
        std::vector<Edge*> m_cntrE; //Traced Contour made of edges (ordered)
        std::vector<Vertex> m_verts; //Contour made of verticies (ordered)
        std::vector<Vertex> m_lockedVerts;
        Contour();
        ~Contour();
        void extratVertexData();
    };
};

#endif
