#ifndef CONTGEN_H_
#define CONTGEN_H_
#include <vector>
#include "Vertex.h"
#include "typedefs.h"
namespace deway
{
    class Vertex;
    class Contour;
    class Edge;
    class Region;
    //typedef std::vector<Edge*> Contour;
    class ContGen 
    {
    public:
        std::vector<Region*> * m_regions_ref;
        std::vector<Edge*> m_edge;
        std::vector<Vertex*> m_vertex; 
        std::vector<Contour*> m_contours;
        ContGen(std::vector<Region*> * _regions);
        ~ContGen();
        void genEdges();
        void flagExtEdges();
        void traceContours();
        
        void extractContourVertex(); //NOT FINSIHED
        void genSegments();
        void RDP_simp(std::vector<Vertex*> * _segment, float _tollerance, uint _start, uint _end);
        void reduceVerts();
    };
};
#endif
