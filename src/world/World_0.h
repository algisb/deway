#ifndef WORLD_0_H
#define WORLD_0_H

#include "World.h"
#include "Shader.h"
#include "Mesh.h"
#include "component/Transform.h"
#include "component/Render.h"
#include "component/Camera.h"
#include "component/Light.h"
#include "component/RenderLine.h"
#include "component/Empty.h"
#include "component/KePhys.h"

namespace deway
{
    class NMGen;
    class PathFinder;
};

namespace kelp
{
    
    class World_0 : public World
    {
    public:
        deway::NMGen * nmgen;
        deway::PathFinder * pathFinder;
        
        Entity * empty[10];
        
        Entity * wall;
        Entity * plight;
        Entity * cube;
        Entity * sphere;
        Entity * camera;
        World_0(Core * _core);
        ~World_0();
        void initW();
        void updateW();
    };
    
};


#endif // WORLD_0_H
