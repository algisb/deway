#include "Vertex.h"
using namespace deway;

Vertex::Vertex()
{
    pos = kep::Vector3(0.0f, 0.0f, 0.0f);
    locked = false;
    parent = NULL;
    G_cost = 0.0f;
    H_cost = 0.0f;
    F_cost = 0.0f;
}
Vertex::~Vertex()
{
}

