#ifndef CONTGEN_H_
#define CONTGEN_H_
#include <vector>
namespace deway
{
    class Edge;
    class Region;
    class ContGen 
    {
    public:
        std::vector<Region*> * m_regions_ref;
        std::vector<Edge*> m_edge;
        ContGen(std::vector<Region*> * _regions);
        ~ContGen();
        void genEdges();
        void flagExtEdges();
    };
};
#endif
