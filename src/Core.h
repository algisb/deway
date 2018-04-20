//      .--..--..--..--..--..--.
//    .' \  (`._   (_)      ---\------------<<<//END ME
//  .'    |  '._)         (_)  |
//  \ _.')\      .----..---.   /
//  |(_.'  |    /    .-\-.  \  |
//  \     0|    |   ( O| O) | o|
//   |  _  |  .--.____.'._.-.  |
//   \ (_) | o         -` .-`  |
//    |    \   |`-._ _ _ _ _\ /
//    \    |   |  `. |_||_|   |     KELP ENGINE
//    | o  |    \_      \     |     -.   .-.
//    |.-.  \     `--..-'   O |     `.`-' .'
//  _.'  .' |     `-.-'      /-.__   ' .-'
//.' `-.` '.|='=.='=.='=.='=|._/_ `-'.'
//`-._  `.  |________/\_____|    `-.'
//   .'   ).| '=' '='\/ '=' |
//   `._.`  '---------------'
//           //___\   //___\
//             ||       ||
//             ||_.-.   ||_.-.
//            (_.--__) (_.--__)
#ifndef CORE_KELP_H_
#define CORE_KELP_H_

#include "GL/glew.h"
#include "Config.h"
#include "Input.h"
#include "Time.h"

#include "World.h"
#include "Entity.h"
#include "component/Transform.h"
#include "kep/Core.h"
#include "world/World_0.h"
#include "Texture.h"
//Fore initialising worlds
namespace kelp
{
    class Core
    {
    public:
        MeshLoad * m_plane;
        MeshLoad * m_sphereMesh;
        MeshLoad * m_cubeMesh;
        MeshLoad * m_monkeyMesh;
        MeshLoad * m_sandBox;
        MeshGen * m_triangleMesh;
        MeshGenLine * m_voxelVolumeMesh;
        MeshGenLine * m_voxelVolumeOutlineMesh;
        MeshLoad * m_externalMesh;
        MeshLoad * m_dungeon;
        MeshGenLine * m_contour;
        MeshGen * m_navMesh;
        
        MeshGenLine * m_testLine;
        
        ShaderMin * m_shaderMinimal;
        ShaderDefault * m_shaderDefault;
        
        
        
        Texture * m_testTexture;
        
        
        World * m_world;
        Core(int _argc, char ** _argv);
        ~Core();
        void update();
    };
};
#endif // CORE_KELP_H_

