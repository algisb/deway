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
#include "deway/AABB.h"
#include "deway/Edge.h"
#include "deway/RegGen.h"
#include <time.h>
#include "deway/ContGen.h"
#include "deway/Contour.h"
#include "deway/Vertex.h"

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
    
    
    plight = new Entity(this, "Point Light");
    plight->addComponent(new Transform(
                                        kep::Vector3(5.0f, 12.0f, 5.0f),
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
        80, 30, 80, //volume dimensions
        kep::Vector3(0.0f, 5.0f, 0.0f), //volume offset
        true, //auto size the volume
        0.2f, //voxel size
        0.8f, //max slope incline
        2.0f, //agent height
        2.0f//max step height
        );
    
    
    m_core->m_voxelVolumeOutlineMesh->addBox(kep::Vector3(nmgen.m_offset.x, 
                                                          nmgen.m_offset.y, 
                                                          nmgen.m_offset.z), 
                                             kep::Vector3(nmgen.m_volX * nmgen.m_voxelSize,
                                                          nmgen.m_volY * nmgen.m_voxelSize,
                                                          nmgen.m_volZ * nmgen.m_voxelSize));
    m_core->m_voxelVolumeOutlineMesh->gen();
    
    
    
    refEntity = new Entity(this, "voxel volume outline");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_voxelVolumeOutlineMesh, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    

    
//     //SINGLE VOXEL VISUALS
//     srand(time(NULL));
//     int rID = rand() % nmgen.m_numTravVoxels;
//     printf("rID: %u \n", rID-1);
//     
//     refEntity = new Entity(this, "quad");
//     refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                         kep::Vector3(nmgen.m_travVoxels[rID]->aabb->c.x, nmgen.m_travVoxels[rID]->aabb->c.y + nmgen.m_voxelSize, nmgen.m_travVoxels[rID]->aabb->c.z),
//                                         kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                         kep::Vector3(nmgen.m_voxelSize, nmgen.m_voxelSize, nmgen.m_voxelSize)
//                                         ));
//     refEntity->addComponent(new Render(m_core->m_plane, m_core->m_shaderMinimal ,NULL, RenderMode::SOLID, kep::Vector3(0.0f, 0.0f, 0.0f)));
//     for(int i = 0; i<3; i=i+1)
//     {
//         refEntity = new Entity(this, "quad");
//         refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                             kep::Vector3(nmgen.m_travVoxels[rID]->nghbr[i]->aabb->c.x, nmgen.m_travVoxels[rID]->nghbr[i]->aabb->c.y + nmgen.m_voxelSize, nmgen.m_travVoxels[rID]->nghbr[i]->aabb->c.z),
//                                             kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                             kep::Vector3(nmgen.m_voxelSize, nmgen.m_voxelSize, nmgen.m_voxelSize)
//                                             ));
//         refTransform->m_position.dump();
//         
//         refEntity->addComponent(new Render(m_core->m_plane, m_core->m_shaderMinimal ,NULL, RenderMode::SOLID, kep::Vector3(1.0f, 0.0f, 0.0f)));
//     }
    
    
    
    //REGION VISUALS
//     srand(time(NULL));
//     for(uint regID = 0; regID<nmgen.m_regGen->m_regions.size(); regID++)
//     {
//         
//         int r0 = rand() % 100;
//         int r1 = rand() % 100;
//         int r2 = rand() % 100;
//         //printf("rand n: %f \n", (float)r/100.0f);
//         
//         kep::Vector3 col = kep::Vector3((float)r0/100.0f, (float)r1/100.0f, (float)r2/100.0f);
//         for(uint i = 0; i<nmgen.m_regGen->m_regions[regID]->m_voxels.size(); i++)
//         {
//             
//             refEntity = new Entity(this, "quad");
//             refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                                 kep::Vector3(nmgen.m_regGen->m_regions[regID]->m_voxels[i]->aabb->c.x, nmgen.m_regGen->m_regions[regID]->m_voxels[i]->aabb->c.y + nmgen.m_voxelSize, nmgen.m_regGen->m_regions[regID]->m_voxels[i]->aabb->c.z),
//                                                 kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                                 kep::Vector3(nmgen.m_voxelSize, nmgen.m_voxelSize, nmgen.m_voxelSize)
//                                                 ));
//             refEntity->addComponent(new Render(m_core->m_plane, m_core->m_shaderMinimal ,NULL, RenderMode::SOLID, col));
//         }
//     }
    
    //EDGE VISUALS
//     for(uint j = 0; j<nmgen.m_contGen->m_contours.size(); j++)
//     {
//         std::vector<deway::Edge*> & edges = nmgen.m_contGen->m_contours[j]->m_cntrE;
//         for(uint i = 0; i < edges.size(); i++)
//         {
//             m_core->m_contour->addLine(edges[i]->vertex[0]->pos, edges[i]->vertex[1]->pos);
//         }
//     }
//     m_core->m_contour->gen();
    
//     for(uint j = 0; j<nmgen.m_contGen->m_regions_ref->size(); j++)
//     {
//         float asd = 0.0f;
//         for(uint k = 0; k<(*nmgen.m_contGen->m_regions_ref)[j]->m_voxels.size(); k++)
//         {
//             for(uint l = 0; l<4; l++)
//             {
//                 //(*nmgen.m_contGen->m_regions_ref)[j]->m_voxels[k]->edg[l];
//                 if((*nmgen.m_contGen->m_regions_ref)[j]->m_voxels[k]->edg[l]->external)
//                 {
//                     //tmpVox.push_back((*nmgen.m_contGen->m_regions_ref)[j]->m_voxels[k]);
//                     //for(uint x = 0; x<4; x++)
//                         m_core->m_contour->addLine( (*nmgen.m_contGen->m_regions_ref)[j]->m_voxels[k]->edg[l]->vertex[0]->pos /*+ kep::Vector3(asd,asd,asd)*/, (*nmgen.m_contGen->m_regions_ref)[j]->m_voxels[k]->edg[l]->vertex[1]->pos/*+ kep::Vector3(asd,asd,asd)*/);
//                     //break;
//                 }
//             }
//             asd = asd + 0.1f;
//         }
// 
//     }
    
    
//     m_core->m_contour->gen();
//     
//     
//     refEntity = new Entity(this, "contour outline");
//     refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                         kep::Vector3(0.0f, 0.0f, 0.0f),
//                                         kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                         kep::Vector3(1.0f, 1.0f, 1.0f)
//                                         ));
//     refEntity->addComponent(new Render(m_core->m_contour, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    
    //CONTOUR VISUALS
    for(uint j = 0; j<nmgen.m_contGen->m_contours.size(); j++)
    {
        std::vector<deway::Vertex*> & verts = nmgen.m_contGen->m_contours[j]->m_reducedVerts;
        for(uint i = 0; i < verts.size(); i++)
        {
            if(i == (verts.size()-1))
                m_core->m_contour->addLine(verts[i]->pos, verts[0]->pos);
            else
                m_core->m_contour->addLine(verts[i]->pos, verts[i+1]->pos);
        }
    }
    m_core->m_contour->gen();
    
    
    refEntity = new Entity(this, "contour outline");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_contour, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    
    
    //NAV-MESH VISUALS
    
    m_core->m_navMesh->addTri(kep::Vector3(3.0f, 5.0f, 6.0f), kep::Vector3(0.0f, 9.0f, -5.0f), kep::Vector3(-6.0f, 4.0f, 8.0f));
    m_core->m_navMesh->gen();
    
    refEntity = new Entity(this, "navMesh");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_navMesh, m_core->m_shaderMinimal, NULL, RenderMode::SOLID, kep::Vector4(0.0f, 1.0f, 0.0f, 0.1f)));
    refEntity = new Entity(this, "navMesh edges");
    refTransform = (Transform*)refEntity->addComponent(new Transform(
                                        kep::Vector3(0.0f, 0.0f, 0.0f),
                                        kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
                                        kep::Vector3(1.0f, 1.0f, 1.0f)
                                        ));
    refEntity->addComponent(new Render(m_core->m_navMesh, m_core->m_shaderMinimal, NULL, RenderMode::WIRE, kep::Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
    
    
    //SEGMENT VISUALS
//     for(uint j = 0; j<nmgen.m_contGen->m_contours.size(); j++)
//     {
//         std::vector<std::vector<deway::Vertex*>> & segs = nmgen.m_contGen->m_contours[j]->m_segments;
//         for(uint m = 0; m<segs.size(); m++)
//         for(uint i = 0; i < segs[m].size()-1; i++)
//         {
//                 m_core->m_contour->addLine(segs[m][i]->pos, segs[m][i+1]->pos);
//         }
//     }
//     m_core->m_contour->gen();
//     
//     
//     refEntity = new Entity(this, "contour outline");
//     refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                         kep::Vector3(0.0f, 0.0f, 0.0f),
//                                         kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                         kep::Vector3(1.0f, 1.0f, 1.0f)
//                                         ));
//     refEntity->addComponent(new Render(m_core->m_contour, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    
    //EXTERNAL EDGE VISUALS
//     for(uint regID = 0; regID<nmgen.m_regGen->m_regions.size(); regID++)
//     {
//         kep::Vector3 col = kep::Vector3(1, 0, 0);
//         for(uint i = 0; i<nmgen.m_regGen->m_regions[regID]->m_voxels.size(); i++)
//         {
//             bool isExt = false;
//             for(uint j = 0; j<4; j++)
//                 if(nmgen.m_regGen->m_regions[regID]->m_voxels[i]->edg[j]->external)
//                 {
//                     m_core->m_contour->addLine(nmgen.m_regGen->m_regions[regID]->m_voxels[i]->edg[j]->v[0], nmgen.m_regGen->m_regions[regID]->m_voxels[i]->edg[j]->v[1]);
//                 }
//         }
//     }
//     m_core->m_contour->gen();
//     refEntity = new Entity(this, "contour outline");
//     refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                         kep::Vector3(0.0f, 0.0f, 0.0f),
//                                         kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                         kep::Vector3(1.0f, 1.0f, 1.0f)
//                                         ));
//     refEntity->addComponent(new Render(m_core->m_contour, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    
    
    
    
    
    //DISTANCE MAP VISUALS
//     for(int i = 0; i<nmgen.m_numTravVoxels; i++)
//     {
//         refEntity = new Entity(this, "quad");
//         refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                             kep::Vector3(nmgen.m_travVoxels[i]->aabb->c.x, nmgen.m_travVoxels[i]->aabb->c.y + nmgen.m_voxelSize, nmgen.m_travVoxels[i]->aabb->c.z),
//                                             kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                             kep::Vector3(nmgen.m_voxelSize, nmgen.m_voxelSize, nmgen.m_voxelSize)
//                                             ));
//         float shade = nmgen.m_travVoxels[i]->dist/nmgen.m_maxEdgeDist;
//         
//         refEntity->addComponent(new Render(m_core->m_plane, m_core->m_shaderMinimal ,NULL, RenderMode::SOLID, kep::Vector3(shade, shade, shade)));
//     }
    
    
    //TEST VOLUME VISUALIZATION USING LINES///////////////////////////////////////////////////////////////////
//     for(int i = 0; i<nmgen.m_numTravVoxels; i++)
//     {
//         m_core->m_voxelVolumeMesh->addBox(nmgen.m_travVoxels[i]->aabb->c, nmgen.m_travVoxels[i]->aabb->hs);
//     }
//     
//     for(int i = 0; i<nmgen.m_numEdgeVoxels; i++)
//     {
//         m_core->m_voxelVolumeMesh->addTopQuad(nmgen.m_edgeVoxels[i]->aabb->c, nmgen.m_edgeVoxels[i]->aabb->hs);
//     }
    
//     for(int i = 0; i<nmgen.m_numVoxel; i++)
//     {
//         m_core->m_voxelVolumeMesh->addBox(nmgen.m_voxels[i].aabb.c, nmgen.m_voxels[i].aabb.hs);
//     }
    
//     for(int i = 0; i<nmgen.m_spans[0].m_size; i++)
//     {
//         m_core->m_voxelVolumeMesh->addBox(nmgen.m_spans[0].m_voxels[i]->aabb.c, nmgen.m_spans[0].m_voxels[i]->aabb.hs);
//     }
    
//     m_core->m_voxelVolumeMesh->gen();
//     
//     refEntity = new Entity(this, "voxel volume");
//     refTransform = (Transform*)refEntity->addComponent(new Transform(
//                                         kep::Vector3(0.0f, 0.0f, 0.0f),
//                                         kep::Quaternion(kep::Vector3(0,1,0), 0.0f), 
//                                         kep::Vector3(1.0f, 1.0f, 1.0f)
//                                         ));
//     refEntity->addComponent(new Render(m_core->m_voxelVolumeMesh, m_core->m_shaderMinimal, NULL, RenderMode::WIRE));
    
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
