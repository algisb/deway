#include "Mesh.h"
using namespace kelp;
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define UNPACK_OBJ(_EXP) \
for (size_t s = 0; s < shapes.size(); s++) \
    {\
        size_t index_offset = 0;\
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)\
        {\
            int fv = shapes[s].mesh.num_face_vertices[f];\
            for (size_t v = 0; v < fv; v++)\
            {\
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];\
                _EXP\
            }\
            index_offset += fv;\
            shapes[s].mesh.material_ids[f];\
        }\
    }
    
Mesh::Mesh()
{
    m_vao = 0;
    m_vboV = 0;
    m_vboN = 0;
    m_vboT = 0;
    m_numVertices = 0;
    m_drawMode = GL_TRIANGLES;
    
}

Mesh::~Mesh()
{
    
}
void Mesh::clear()
{
    if(m_vboV != 0)
        glDeleteBuffers(1, &m_vboV);
    if(m_vboN != 0)
        glDeleteBuffers(1, &m_vboN);
    if(m_vboT != 0)
        glDeleteBuffers(1, &m_vboT);
    
    
    if(m_vao != 0)
        glDeleteVertexArrays(1, &m_vao);
    m_numVertices = 0;
}


MeshGen::MeshGen()
{
    m_drawMode = GL_TRIANGLES;
}
MeshGen::~MeshGen()
{
    clear();
}

void MeshGen::addTri(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _p2)
{
    m_verticies.push_back(_p0);
    m_verticies.push_back(_p1);
    m_verticies.push_back(_p2);
}
void MeshGen::addBox(kep::Vector3 _center, kep::Vector3 _halfSize)
{
    kep::Vector3 p[8];
    p[0] = _center + kep::Vector3(_halfSize.x, _halfSize.y, _halfSize.z);
    p[1] = _center + kep::Vector3(-_halfSize.x, _halfSize.y, _halfSize.z);
    p[2] = _center + kep::Vector3(-_halfSize.x, _halfSize.y, -_halfSize.z);
    p[3] = _center + kep::Vector3(_halfSize.x, _halfSize.y, -_halfSize.z);

    p[4] = _center + kep::Vector3(_halfSize.x, -_halfSize.y, _halfSize.z);
    p[5] = _center + kep::Vector3(-_halfSize.x, -_halfSize.y, _halfSize.z);
    p[6] = _center + kep::Vector3(-_halfSize.x, -_halfSize.y, -_halfSize.z);
    p[7] = _center + kep::Vector3(_halfSize.x, -_halfSize.y, -_halfSize.z);
    
    addTri(p[0], p[1], p[3]);
    addTri(p[2], p[1], p[3]);

    addTri(p[4], p[5], p[7]);
    addTri(p[6], p[5], p[7]);    
    
    addTri(p[0], p[1], p[4]);
    addTri(p[5], p[1], p[4]);
    
    addTri(p[1], p[2], p[5]);
    addTri(p[6], p[2], p[5]);
    
    addTri(p[2], p[3], p[6]);
    addTri(p[7], p[3], p[6]);
    
    addTri(p[3], p[0], p[7]);
    addTri(p[4], p[0], p[7]);
    
}
void MeshGen::gen()
{
    ///////////////////////////////////
    clear();
    ///////////////////////////////////
    
    if(m_verticies.size()>0)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        m_numVertices = m_verticies.size();
        glGenBuffers(1, &m_vboV);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kep::Vector3) * m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

MeshGenLine::MeshGenLine()
{
    m_drawMode = GL_LINES;
}
MeshGenLine::~MeshGenLine()
{
    clear();
}

void MeshGenLine::addLine(kep::Vector3 _p0, kep::Vector3 _p1)
{
    m_verticies.push_back(_p0);
    m_verticies.push_back(_p1);
}
void MeshGenLine::addBox(kep::Vector3 _center, kep::Vector3 _halfSize)
{
    kep::Vector3 p[8];
    p[0] = _center + kep::Vector3(_halfSize.x, _halfSize.y, _halfSize.z);
    p[1] = _center + kep::Vector3(-_halfSize.x, _halfSize.y, _halfSize.z);
    p[2] = _center + kep::Vector3(-_halfSize.x, _halfSize.y, -_halfSize.z);
    p[3] = _center + kep::Vector3(_halfSize.x, _halfSize.y, -_halfSize.z);

    p[4] = _center + kep::Vector3(_halfSize.x, -_halfSize.y, _halfSize.z);
    p[5] = _center + kep::Vector3(-_halfSize.x, -_halfSize.y, _halfSize.z);
    p[6] = _center + kep::Vector3(-_halfSize.x, -_halfSize.y, -_halfSize.z);
    p[7] = _center + kep::Vector3(_halfSize.x, -_halfSize.y, -_halfSize.z);
    
    addLine(p[0], p[1]);
    addLine(p[1], p[2]);
    addLine(p[2], p[3]);
    addLine(p[3], p[0]);
    
    addLine(p[4], p[5]);
    addLine(p[5], p[6]);
    addLine(p[6], p[7]);
    addLine(p[7], p[4]);
    
    addLine(p[0], p[4]);
    addLine(p[1], p[5]);
    addLine(p[2], p[6]);
    addLine(p[3], p[7]);
}

void MeshGenLine::gen()
{
    ///////////////////////////////////
    clear();
    ///////////////////////////////////
    if(m_verticies.size()>0)
    {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        m_numVertices = m_verticies.size();
        glGenBuffers(1, &m_vboV);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kep::Vector3) * m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}




                

MeshLoad::MeshLoad(const char * _objPath, const char * _mtlDir)
{
    m_drawMode = GL_TRIANGLES;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, _objPath, _mtlDir);
    
    if (!err.empty())
        printf("%s \n", err.c_str());
        

    if (!ret) 
        exit(1);

    
    
    for (size_t s = 0; s < shapes.size(); s++) 
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            int fv = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; v++)
            {
                m_numVertices++;
            }
        }
    }
    
    
    if(attrib.vertices.size() != 0)
    {
        m_dataV = new float[m_numVertices*3];
        size_t dataV_i = 0;
        UNPACK_OBJ(
                    for(size_t i = 0; i<3; i++)
                    {
                        m_dataV[dataV_i] = attrib.vertices[3*idx.vertex_index+i];
                        dataV_i++;
                    }
                    );


        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vboV);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, m_dataV, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        

        //delete data from heap
        //delete[] dataV;


    }
    

    m_dataN = new float[m_numVertices*3];
    size_t dataN_i = 0;
    if(attrib.normals.size() != 0)
    {
    UNPACK_OBJ(
                for(size_t i = 0; i<3; i++)
                {
                    
                    m_dataN[dataN_i] = attrib.normals[3*idx.normal_index+i];
                    dataN_i++;
                }
                //printf("%f %f %f \n", attrib.normals[3*idx.normal_index+0], attrib.normals[3*idx.normal_index+1], attrib.normals[3*idx.normal_index+2]);
                );
    }
    else//gen normals
    {
        
        for(int i = 0; i< m_numVertices*3; i = i + 3*3)
        {
            kep::Vector3 p[3];
                for(int k = 0; k<9; k = k + 3)
                    p[k/3] = kep::Vector3(m_dataV[i+k+0], m_dataV[i+k+1], m_dataV[i+k+2]);
                
            kep::Vector3 v1 = p[1] - p[0];
            kep::Vector3 v2 = p[2] - p[0];
            kep::Vector3 n = kep::cross(v1,v2).normalized();
            
            for(int k = 0; k<9; k = k + 3)
                for(int j = 0; j<3; j++)
                m_dataN[i+k+j] = n.data[j];
        }
    }
    glGenBuffers(1, &m_vboN);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboN);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 3, m_dataN, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    //delete data from heap
    //delete[] dataN;
    
    if(attrib.texcoords.size() != 0)
    {
        float * dataT = new float[m_numVertices*2];
        size_t dataT_i = 0;
        
        UNPACK_OBJ(
                    for(size_t i = 0; i<2; i++)
                    {
                        dataT[dataT_i] = attrib.texcoords[2*idx.texcoord_index+i];
                        dataT_i++;
                    }
                  );
        glGenBuffers(1, &m_vboT);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboT);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_numVertices * 2, dataT, GL_STATIC_DRAW);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);
        
        //delete data from heap
        delete[] dataT;
    }
    
    //unbind evrything////////////////////////////////////
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    

}

MeshLoad::~MeshLoad()
{
    clear();
    delete[] m_dataV;
    delete[] m_dataN;
}

