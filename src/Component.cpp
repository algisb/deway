///
///  @file Component.cpp
///  @brief abstract class for deriving component types which make mup entities

#include "Component.h"

using namespace kelp;

Component::Component()
{
    m_enabled = true;
}

Component::~Component()
{
    
}
void Component::render()
{
    
}
