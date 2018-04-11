#include "Edge.h"
using namespace deway;
Edge::Edge()
{
    external = false;
    v[0] = kep::Vector3();
    v[1] = kep::Vector3();
    nghbr[0] = NULL;
    nghbr[1] = NULL;
    creator = NULL;
}

Edge::~Edge()
{
}

