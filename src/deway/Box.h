#ifndef BOX_DEWAY_H_
#define BOX_DEWAY_H_
#include "Core.h"
namespace deway 
{
    class Box
    {
    public:
        kep::Vector3 c;//center
        kep::Vector3 hs;//halfsize
        Box(kep::Vector3 _center = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _halfSize = kep::Vector3(0.0f, 0.0f, 0.0f));
        ~Box();
        
    };
};

#endif //BOX_DEWAY_H_
