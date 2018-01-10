#include "Collider.h"
#include "../../RigidBody.h"
using namespace kep;
Collider::Collider(Matrix4 _offset)
{
    rigidBody = NULL;
    offset = _offset;
    maxReach = 1.0f;
}
Collider::~Collider()
{
}
void Collider::update()
{
}

