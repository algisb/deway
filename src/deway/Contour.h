#ifndef CONTOUR_DEWAY_H_
#define CONTOUR_DEWAY_H_
#include <stdlib.h>
#include <vector>
#include "Vertex.h"

namespace deway 
{
    class Region;
    class Edge;
    class Contour 
    {
    public:
        Region * reg;//region from which the contour was traced
        std::vector<Edge*> m_cntrE; //Traced Contour made of edges (ordered)
        std::vector<Vertex*> m_verts; //Contour made of verticies (ordered)
        std::vector<std::vector<Vertex*>> m_segments; // segments rdy to be reduced
        std::vector<Vertex*> m_reducedVerts;
        
        Contour();
        ~Contour();
    };
};

#endif
