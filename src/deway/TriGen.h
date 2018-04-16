#ifndef TRIGEN_H_
#define TRIGEN_H_
#include <vector>
#include "typedefs.h"

namespace deway
{
    class TriangleO;
    class Contour;
    class TriGen 
    {
    public:
        std::vector<Contour*> * m_contours_ref;
        std::vector<TriangleO*> m_navMesh;
        
        TriGen(std::vector<Contour*> * _contours);
        ~TriGen();
        void earClip();
        void genTriangles();
        
        
    };
};

#endif //TRIGEN_H_
