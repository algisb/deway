#ifndef DEWAY_RAY_H_
#define DEWAY_RAY_H_
#include "Core.h"
namespace deway 
{
    class Ray
    {
    public:
        kep::Vector3 s;
        kep::Vector3 d;
        Ray(kep::Vector3 _source = kep::Vector3(0.0f, 0.0f, 0.0f), kep::Vector3 _direction = kep::Vector3(0.0f, 0.0f, 1.0f));
        ~Ray();
        
    };
};

#endif //DEWAY_RAY_H_
