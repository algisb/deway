#ifndef COLLISIONDATA_H_
#define COLLISIONDATA_H_
#include "Contact.h"
#include <vector>
namespace kep
{
    class CollisionData
    {
    public:
        std::vector<Contact> contacts;
        CollisionData();
        ~CollisionData();
    };
};

#endif //COLLISIONDATA_H_
