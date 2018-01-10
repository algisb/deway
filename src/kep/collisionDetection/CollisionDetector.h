#ifndef COLLISIONDETECTOR_H_
#define COLLISIONDETECTOR_H_
#include "../RigidBody.h"
#include "finephase/CollisionData.h"
#include "broadphase/PotencialContact.h"
#include <vector>
namespace kep
{
    class CollisionDetector
    {
    public:
        std::vector<RigidBody*> * rigidBodies;
        CollisionDetector(std::vector<RigidBody*> * _rigidBodies);
        ~CollisionDetector();
        void detect(CollisionData * _cd);
    private:
        void broadPhase(std::vector<PotencialContact> * _pc);
        void finePhase(CollisionData * _cd, std::vector<PotencialContact> * _pc);
    };
};

#endif // COLLISIONDETECTOR_H_
