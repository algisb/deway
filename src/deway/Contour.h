#ifndef CONTOUR_DEWAY_H_
#define CONTOUR_DEWAY_H_
#include <stdlib.h>
#include <vector>
namespace deway 
{
    class Edge;
    class Contour 
    {
    public:
        std::vector<Edge*> m_contour;
        Contour();
        ~Contour();
    };
};

#endif
