///
///  @file World_0.cpp
///  @brief holds all the entities that get created also loads assets and other shared objects

#include "World_0.h"
#include "Core.h"
#include "kep/collisionDetection/finephase/Collider.h"
#include "kep/collisionDetection/finephase/SphereCollider.h"
#include "kep/collisionDetection/finephase/HalfPlaneCollider.h"
#include "kep/collisionDetection/finephase/OBBCollider.h"
#include "kep/collisionDetection/finephase/MeshCollider.h"
#include "deway/NMGen.h"
using namespace kelp;

World_0::World_0(Core * _core) : World(_core)
{
    Entity * refEntity = NULL;
    Transform * refTransform = NULL;
    kep::MeshCollider * mc = NULL;
    
    //LIGHTS//////////////////////////////////////////////////////////////////////////////////////
    refEntity = new Entity(this, "Directional Light");
    refEntity->addComponent(new LightDirectional(m_core->m_shaderDefault, 0.5f, kep::Vector3(0.0f,0.0f,0.0f), kep::Vector3(0.0f, 1.0f, 1.0f)));
    
    plight = new Entity(this, "Point Light");
    plight->addComponent(new Transform(
                                        kep::Vector3(5.0f, 10.0f, 5.0f),
                                        kep::Quaternion(), 
                                        kep::Vector3(0.2f, 0.2f, 0.2f)
                                        ));
    plight->addComponent(new LightPoint(m_core->m_shaderDefault, 50.0f, kep::Vector3(0.0f, 0.0f, 0.0f)));
    plight->addComponent(new Render(m_core->m_sphereMesh, m_core->m_shaderMinimal, NULL, RenderMode::SOLID));
    
   /////////////////////////////////////////////////////////////////////////
    
    //TEST TRIANGLE
    
    Mesh * polySoup = /*m_core->m_triangleMesh;*/m_core->m_sandBox; //<----------------------------------------------------
    
    m_core->m_triangleMesh->addTri(kep::Vector3(3.0f, 5.0f, 6.0f), kep::Vector3(0.0f, 9.0f, -5.0f), kep::Vector3(-6.0f, 4.0f, 8.0f));
    m_core->m_triangleMesh->gen();
    
    refEntity = new Entity(this, "sandbox");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(polySoup, m_core->m_shaderDefault, NULL, RenderMode::SOLID));
    
    //GENERATOR
    
    deway::NMGen nmgen(polySoup->m_dataV, polySoup->m_dataN, polySoup->m_numVertices,
        80, 30, 80, false, //volume dimensions
        0.3f, //voxel size
        0.8f, //max slope incline
        kep::Vector3(0.0f, 5.0f, 0.0f));
    
    
    m_core->m_voxelVolumeOutlineMesh->addBox(kep::Vector3(nmgen.m_offset.x, 
                                                          nmgen.m_offset.y, 
                                                          nmgen.m_offset.z), 
                                             kep::Vector3(nmgen.m_volX * nmgen.m_voxelSize,
                                                          nmgen.m_volY * nmgen.m_voxelSize,
                                                          nmgen.m_volZ * nmgen.m_voxelSize));
    m_core->m_voxelVolumeOutlineMesh->gen();
    
    
//     m_core->m_voxelVolumeOutlineMesh->addBox(kep::Vector3(nmgen.m_triangles[0].aabb->c.x, 
//                                                           nmgen.m_triangles[0].aabb->c.y, 
//                                                           nmgen.m_triangles[0].aabb->c.z), 
//                                              kep::Vector3(nmgen.m_triangles[0].aabb->hs.x,
//                                                           nmgen.m_triangles[0].aabb->hs.y,
//                                                           nmgen.m_triangles[0].aabb->hs.z));
//     m_core->m_voxelVolumeOutlineMesh->gen();
    
    
    refEntity = new Entity(this, "voxel volume outline");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_voxelVolumeOutlineMesh, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    
    //TEST VOLUME VISUALIZATION
    for(int i = 0; i<nmgen.m_numOverlapVoxels; i++)
    {
        m_core->m_voxelVolumeMesh->addTopQuad(nmgen.m_overlapVoxels[i]->aabb.c, nmgen.m_overlapVoxels[i]->aabb.hs);
    }
    
//     for(int i = 0; i<nmgen.m_numVoxel; i++)
//     {
//         m_core->m_voxelVolumeMesh->addBox(nmgen.m_voxels[i].aabb.c, nmgen.m_voxels[i].aabb.hs);
//     }
    
    m_core->m_voxelVolumeMesh->gen();
    
    refEntity = new Entity(this, "voxel volume");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_voxelVolumeMesh, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    //////////////////////CAM/////////////////////////
    empty[1] = new Entity(this, "player camera");
    empty[1]->addComponent(new Transform(
                                    kep::Vector3(0.0f, 10.0f, 0.0f),
                                    kep::Quaternion(), 
                                    kep::Vector3(1.0f, 1.0f, 1.0f)
                                    ));
    
    
    empty[1]->addComponent(
                            new Camera(kep::Vector3(0.0f, 1.0f, 0.0f),
                            kep::perspectiveProjection(
                                45.0f, 
                                Config::s_windowWidth, 
                                Config::s_windowHeight, 
                                1.0f, 
                                1000.0f).transpose(),
                                true
                            ));
    refEntity = new Entity(this, "empty");//empty structure for having global components and testing stuff
    refEntity->addComponent(new Empty(empty[1]));
}
World_0::~World_0()
{
    
}

void World_0::initW()
{
    //Time::s_deltaT = 0.01666f;//delta time on init is bad
    //KePhys * kePhys = cube->getComponent<KePhys>();
    
    //kePhys->m_rigidBody->addTorque(kep::Vector3(0,0,1000));
    //kePhys->m_rigidBody->addForceAtBodyPoint(kep::Vector3(0,0,-100), kep::Vector3(1,0,0));
    //kePhys->m_rigidBody->addForce(kep::Vector3(0, -1000, 0));
    //kePhys->m_rigidBody->addForce(kep::Vector3(500, 0, 0));
    //kePhys->m_rigidBody->addForce(kep::Vector3(500, 0, 0))
}

void World_0::updateW()
{
    //KePhys * kePhys = cube->getComponent<KePhys>();
    
    //kePhys->m_rigidBody->velocity.dump();
    //kePhys->m_rigidBody->acceleration.dump();
    //empty[0]->getComponent<Transform>()->dump();
    //kePhys->m_rigidBody->angularVelocity.dump();
}
