#ifndef DEWAY_LOC_H_
#define DEWAY_LOC_H_
#include "Core.h"

namespace deway 
{
    class TriangleO;
    class Loc 
    {
    public:
        TriangleO * tri;
        kep::Vector3 pos;
        
        Loc(TriangleO * _tri = NULL, kep::Vector3 _pos = kep::Vector3(0.0f, 0.0f, 0.0f));
        ~Loc();
        
    };
}

#endif //DEWAY_LOC_H_
