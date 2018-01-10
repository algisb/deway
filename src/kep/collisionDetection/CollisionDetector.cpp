#include "CollisionDetector.h"
#include "broadphase/BoundingSphere.h"
#include "finephase/SphereCollider.h"
using namespace kep;

CollisionDetector::CollisionDetector(std::vector<RigidBody*> * _rigidBodies)
{
    rigidBodies = _rigidBodies;
}
CollisionDetector::~CollisionDetector()
{
}
void CollisionDetector::detect(CollisionData* _cd)
{
    std::vector<PotencialContact> pc;
    broadPhase(&pc);
    finePhase(_cd, &pc);
}

void CollisionDetector::broadPhase(std::vector<PotencialContact> * _pc)
{
    for(uint64_t i = 0; i < rigidBodies->size(); i++)
        for(uint64_t j = 0; j < rigidBodies->size(); j++)
        {
            if(i==j)//dont check collision against self
                continue;
            bool exists = false;
            for(uint64_t k = 0; k < _pc->size(); k++)
                if(((*_pc)[k].rigidBody[0] == (*rigidBodies)[i] && (*_pc)[k].rigidBody[1] == (*rigidBodies)[j]) || 
                   ((*_pc)[k].rigidBody[1] == (*rigidBodies)[i] && (*_pc)[k].rigidBody[0] == (*rigidBodies)[j])
                   )
                {
                    exists = true;
                    break;
                }
                if(exists)
                    continue;
            if((*rigidBodies)[i]->boundingVolume->overlaps((*rigidBodies)[j]->boundingVolume))
                _pc->push_back(PotencialContact((*rigidBodies)[i], (*rigidBodies)[j]));
        }
}
void CollisionDetector::finePhase(CollisionData * _cd, std::vector<PotencialContact> * _pc)
{
    for(int i = 0; i < _pc->size(); i++)
        (*_pc)[i].rigidBody[0]->collider->collides((*_pc)[i].rigidBody[1]->collider, _cd);//Double dynamic dispatch aka C++ magic
}

