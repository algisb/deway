#include "PotencialContact.h"
using namespace kep;

PotencialContact::PotencialContact(RigidBody* _rb0, RigidBody* _rb1)
{
    rigidBody[0] = _rb0;
    rigidBody[1] = _rb1;
}
PotencialContact::~PotencialContact()
{
}

