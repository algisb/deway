#ifndef CONTACT_H_
#define CONTACT_H_
#include "../../Core.h"
#include "../../RigidBody.h"
namespace kep
{
    class Contact
    {
    public:
        RigidBody * body[2];
        Vector3 position;
        Vector3 normal;
        real penetration;
        
        Matrix3 contactToWorld;
        
        Contact();
        ~Contact();
        void calculateContactBasis();
        void resolve(real _duration);

    };
};

#endif // CONTACT_H_
