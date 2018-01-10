#ifndef KEPWORLD_H_
#define KEPWORLD_H_

#include "RigidBody.h"
#include "ForceGenerator.h"
#include "collisionDetection/CollisionDetector.h"
#include "ContactResolver.h"
namespace kep
{
    class World
    {
    public:
        real targetTimeStep;
        real timeStepAccumulator;
        std::vector<RigidBody*> rigidBodies;
        ForceRegistry * fReg;
        Gravity * gGen;
        LinearDrag * ldGen;
        AngularDrag * adGen;
        CollisionDetector * cDetec;
        ContactResolver * cRes;
        World();
        ~World();
        void update(real _deltaT);
        RigidBody * addRigidBody(RigidBody * _rigidBody);
    };
}

#endif //KEPWORLD_H_
