#ifndef CONTGEN_H_
#define CONTGEN_H_
#include <vector>
namespace deway
{
    class Contour;
    class Edge;
    class Region;
    //typedef std::vector<Edge*> Contour;
    class ContGen 
    {
    public:
        std::vector<Region*> * m_regions_ref;
        std::vector<Edge*> m_edge;
        std::vector<Contour*> m_contours;
        ContGen(std::vector<Region*> * _regions);
        ~ContGen();
        void genEdges();
        void flagExtEdges();
        void traceContours();
        void extractContourVertex();
    };
};
#endif
