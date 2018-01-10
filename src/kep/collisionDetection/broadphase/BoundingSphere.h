#ifndef BOUNDINGSPHERE_H_
#define BOUNDINGSPHERE_H_
#include "../../Core.h"
namespace kep
{
    class RigidBody;
    class BoundingSphere
    {
    public:
        RigidBody * rigidBody;
        real radius;
        BoundingSphere(RigidBody * _rigidBody = NULL, real _radius = 1.0f);
        ~BoundingSphere();
        bool overlaps(BoundingSphere *_other);
    };
};



#endif //BOUNDINGSPHERE_H_
