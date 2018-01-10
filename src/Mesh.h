#ifndef MESH_H_
#define MESH_H_
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "GL/glew.h"
#include "kep/Core.h"


namespace kelp
{
    class Mesh
    {
    public:
        GLuint m_vao;
        GLuint m_vboV;//vertex data
        GLuint m_vboN;//normal data
        GLuint m_vboT;//Texture data
        
        unsigned int m_numVertices;
        float * m_dataV;
        float * m_dataN;
        
        Mesh();
        virtual ~Mesh()=0;
        void clear();
        
    };
    
    class MeshGen : public Mesh
    {
    public:
        std::vector<kep::Vector3> m_verticies;
        MeshGen();
        ~MeshGen();
        void addTri(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _p2);
        void gen();
    };
    
    class MeshLoad : public Mesh
    {
    public:
        MeshLoad(const char * _objPath, const char * _mtlDir);
        ~MeshLoad();
    };
};



#endif // MESH_H_
