#ifndef POTENCIALCONTACT_H_
#define POTENCIALCONTACT_H_
#include "../../RigidBody.h"
namespace kep
{
    class PotencialContact 
    {
    public:
        RigidBody * rigidBody[2];
        PotencialContact(RigidBody * _rb0, RigidBody * _rb1);
        ~PotencialContact();
    };
};

#endif //POTENCIALCONTACT_H_
