///
///  @file Core.cpp
///  @brief core engine components really generic, can be reused in multiple worlds

#include "Core.h"


using namespace kelp;

Core::Core(int _argc, char ** _argv)
{
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    ///////////////////////////////////////
    //MeshGen * meshGen = new MeshGen();
    //meshGen->addTri(kep::Vector3(-2.0,0,0), kep::Vector3(2.0f,0,0), kep::Vector3(0,3.0f,0));
    //meshGen->gen();
    
    
    
    m_plane = new MeshLoad("./models/plane.obj", "./models/");
    m_sphereMesh = new MeshLoad("./models/sphereUV.obj", "./models/");
    m_cubeMesh = new MeshLoad("./models/cube.obj", "./models/");
    m_monkeyMesh = new MeshLoad("./models/monkey.obj", "./models/");
    m_dungeon = new MeshLoad("./models/dungeon.obj", "./models/");
    m_sandBox = new MeshLoad("./models/sandbox.obj", "./models/");//nav_test.obj
    m_triangleMesh = new MeshGen();
    m_voxelVolumeMesh = new MeshGenLine();
    m_voxelVolumeOutlineMesh = new MeshGenLine();
    m_contour = new MeshGenLine();
    m_navMesh = new MeshGen();
    
    m_shaderMinimal = new ShaderMin();
    m_shaderDefault = new ShaderDefault();
    
    m_testTexture = new Texture("./textures/uv.png");
    
    m_world = new World_0(this);
}
Core::~Core()
{
    delete m_plane;
    delete m_sphereMesh;
    delete m_cubeMesh;
    delete m_monkeyMesh;
    delete m_dungeon;
    delete m_sandBox;
    delete m_triangleMesh;
    delete m_voxelVolumeMesh;
    delete m_voxelVolumeOutlineMesh;
    delete m_contour;
    delete m_navMesh;
    
    delete m_shaderMinimal;
    delete m_shaderDefault;
    
    delete m_testTexture;
    
    delete m_world;
}

void Core::update()
{
    //printf("fps: %d \n", Time::s_frameRate);
    //printf("deltaT: %f \n", kelp::Time::s_deltaT);
    //Input::printKeyList();
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //render to the original buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    //update worlds (includes rendering)
    m_world->update();
}
