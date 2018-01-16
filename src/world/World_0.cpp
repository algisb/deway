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
#include "deway/TriBoxTest.h"
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
                                        kep::Vector3(0.0f, 10.0f, 0.0f),
                                        kep::Quaternion(), 
                                        kep::Vector3(0.2f, 0.2f, 0.2f)
                                        ));
    plight->addComponent(new LightPoint(m_core->m_shaderDefault, 50.0f, kep::Vector3(0.0f, 0.0f, 0.0f)));
    plight->addComponent(new Render(m_core->m_sphereMesh, m_core->m_shaderMinimal, NULL, RenderMode::SOLID));
    
   /////////////////////////////////////////////////////////////////////////
    refEntity = new Entity(this, "sandbox");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(10.0f, 10.0f, 10.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_sandBox, m_core->m_shaderDefault, NULL, RenderMode::SOLID));
    
    mc = new kep::MeshCollider(kep::Matrix4(), m_core->m_sandBox->m_dataV, m_core->m_sandBox->m_dataN, m_core->m_sandBox->m_numVertices, refTransform->m_scale);
    refEntity->addComponent(new KePhys(
        m_physWorld->addRigidBody(new kep::RigidBody(&refTransform->m_position, &refTransform->m_orientation, true, 0.0f, mc))//new kep::HalfPlaneCollider()
    ));
    
    //TEST TRIANGLE
    kep::Vector3 v[3] = {kep::Vector3(3.0f, 5.0f, 6.0f), kep::Vector3(0.0f, 9.0f, -5.0f), kep::Vector3(-6.0f, 4.0f, 8.0f)};
    m_core->m_triangleMesh->addTri(v[0],v[1],v[2]);
    m_core->m_triangleMesh->gen();
    
    refEntity = new Entity(this, "test mesh");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_triangleMesh, m_core->m_shaderDefault, NULL, RenderMode::SOLID));
    
    //printf("%f \n", m_core->m_triangleMesh->m_dataV[0]);
    //GENERATOR
    deway::NMGen nmgen(m_core->m_triangleMesh->m_dataV, m_core->m_triangleMesh->m_dataN, m_core->m_triangleMesh->m_numVertices,
        30, 30, 30, 0.5f);
    
    
    //TEST VOLUME VISUALIZATION
    for(int i = 0; i<nmgen.m_numOverlapVoxel; i++)
    {
        m_core->m_voxelVolumeMesh->addBox(nmgen.m_overlapVoxels[i].c, nmgen.m_overlapVoxels[i].hs);
    }
    
//     for(int i = 0; i<nmgen.m_numVoxel; i++)
//     {
//         m_core->m_voxelVolumeMesh->addBox(nmgen.m_voxelVolume[i].c, nmgen.m_voxelVolume[i].hs);
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
