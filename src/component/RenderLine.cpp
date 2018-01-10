#include "RenderLine.h"
#include "Camera.h"

using namespace kelp;
std::vector<kep::Vector3> RenderLine::m_verticies = std::vector<kep::Vector3>();
std::vector<kep::Vector3> RenderLine::m_colours = std::vector<kep::Vector3>();

RenderLine::RenderLine(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _colour)
{
    m_p0 = _p0;
    m_p1 = _p1;
    m_enabled = true;
    m_colour = _colour;
}
RenderLine::~RenderLine()
{
    
}

void RenderLine::renderLines(Shader * _shader, Camera * _rc)
{
//     struct VecCol
//     {
//         kep::Vector3 col;
//         
//     };
    //DO NOT draw heckin rainbows, the more different colours the slower this works
    
    GLuint vao = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    int numVertices = m_verticies.size();

    if(numVertices > 0)
    {
        //totalVertex = totalVertex + _numVertices;
        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, &m_verticies[0], GL_DYNAMIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);


        glUseProgram(_shader->m_shaderLocation);
        kep::Matrix4 m4;
        glUniformMatrix4fv(_shader->m_shaderModelMatLocation, 1, 
                        GL_FALSE, &m4.d[0][0]);
        glUniformMatrix4fv(_shader->m_shaderViewMatLocation, 1, 
                        GL_FALSE, &_rc->m_viewMat.d[0][0]);
        glUniformMatrix4fv(_shader->m_shaderProjMatLocation, 1, 
                        GL_FALSE, &_rc->m_projectionMat.d[0][0]);
        glUniform3fv(_shader->m_shaderColourLocation, 1, kep::Vector3(0.0f, 1.0f, 0.0f).data);
        
        // Tell OpenGL to draw it
        // Must specify the type of geometry to draw and the number of vertices
        glDrawArrays(GL_LINES, 0, numVertices);
        // Unbind VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        m_verticies.clear();
        m_colours.clear();
    }
}

void RenderLine::init()
{
    
}
void RenderLine::update()
{
    if(m_enabled)
    {
        m_verticies.push_back(m_p0);
        m_verticies.push_back(m_p1);
        m_colours.push_back(m_colour);
    }
}
void RenderLine::render()
{
    
}

std::vector<kep::Vector3> RenderLine2::m_verticies = std::vector<kep::Vector3>();
std::vector<kep::Vector3> RenderLine2::m_colours = std::vector<kep::Vector3>();

RenderLine2::RenderLine2(kep::Vector3 _p0, kep::Vector3 _p1, kep::Vector3 _colour)
{
    m_p0 = _p0;
    m_p1 = _p1;
    m_enabled = true;
    m_colour = _colour;
}
RenderLine2::~RenderLine2()
{
    
}

void RenderLine2::renderLines(Shader * _shader, Camera * _rc)
{
//     struct VecCol
//     {
//         kep::Vector3 col;
//         
//     };
    //DO NOT draw heckin rainbows, the more different colours the slower this works
    
    GLuint vao = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    int numVertices = m_verticies.size();
    
    if(numVertices > 0)
    {
        //totalVertex = totalVertex + _numVertices;
        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, &m_verticies[0], GL_DYNAMIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);


        glUseProgram(_shader->m_shaderLocation);
        kep::Matrix4 m4;
        glUniformMatrix4fv(_shader->m_shaderModelMatLocation, 1, 
                        GL_FALSE, &m4.d[0][0]);
        glUniformMatrix4fv(_shader->m_shaderViewMatLocation, 1, 
                        GL_FALSE, &m4.d[0][0]);//_rc->m_viewMat.d[0][0]
        glUniformMatrix4fv(_shader->m_shaderProjMatLocation, 1, 
                        GL_FALSE, &_rc->m_projectionMat.d[0][0]);
        glUniform3fv(_shader->m_shaderColourLocation, 1, kep::Vector3(1.0f, 0.0f, 0.0f).data);
        
        // Tell OpenGL to draw it
        // Must specify the type of geometry to draw and the number of vertices
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_LINES, 0, numVertices);
        glEnable(GL_DEPTH_TEST);
        // Unbind VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        m_verticies.clear();
        m_colours.clear();
    }
}

void RenderLine2::init()
{
    
}
void RenderLine2::update()
{
    if(m_enabled)
    {
        m_verticies.push_back(m_p0);
        m_verticies.push_back(m_p1);
        m_colours.push_back(m_colour);
    }
}
void RenderLine2::render()
{
    
}
