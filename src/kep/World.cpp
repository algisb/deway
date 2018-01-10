#include "World.h"
#include "collisionDetection/broadphase/PotencialContact.h"
#include "collisionDetection/broadphase/BoundingSphere.h"
#include "collisionDetection/finephase/CollisionData.h"
#include "collisionDetection/finephase/SphereCollider.h"
using namespace kep;

World::World()
{
    targetTimeStep = 1.0f/60.0f;//physics updates 60 times a second(EVEN IF YOU RUN ON TOASTER)
    timeStepAccumulator = 0.0f;
    
    fReg = new ForceRegistry();
    
    gGen = new Gravity(Vector3(0.0f, -29.81f, 0.0f));
    ldGen = new LinearDrag(0.5f, 0.01f);
    //adGen = new AngularDrag(0.5, 0.1f);
    adGen = new AngularDrag(0.1f, 0.001f);
    
    cDetec = new CollisionDetector(&rigidBodies);
    cRes = new ContactResolver();
}
World::~World()
{
    for(int i = 0; i < rigidBodies.size(); i++)
        delete rigidBodies[i];
    delete fReg;
    delete gGen;
    delete ldGen;
    delete adGen;
    delete cDetec;
    delete cRes;
}
void World::update(real _deltaT)
{
    timeStepAccumulator += _deltaT;
    while ( timeStepAccumulator >= targetTimeStep )
    {
        fReg->updateForces(targetTimeStep);
        for(int i = 0; i < rigidBodies.size(); i++)
            rigidBodies[i]->integrate(targetTimeStep);
        
        CollisionData cd;
        cDetec->detect(&cd);
    
        cRes->resolve(&cd, targetTimeStep);
        
        //////////////////////////////////////
        timeStepAccumulator -= targetTimeStep;
    }
}
RigidBody * World::addRigidBody(RigidBody * _rigidBody)
{
    fReg->add(_rigidBody, gGen);
    fReg->add(_rigidBody, ldGen);
    fReg->add(_rigidBody, adGen);
    rigidBodies.push_back(_rigidBody);
    return _rigidBody;
}
