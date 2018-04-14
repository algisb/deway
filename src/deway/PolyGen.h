#ifndef POLYGEN_H_
#define POLYGEN_H_
#include <vector>
namespace deway
{
    class Contour;
    class PolyGen 
    {
    public:
        std::vector<Contour*> * m_contours_ref;
        
        PolyGen(std::vector<Contour*> * _contours);
        ~PolyGen();
        
        
    };
};

#endif //POLYGEN_H_
