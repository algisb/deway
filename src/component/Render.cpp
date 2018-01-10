///
///  @file World.cpp
///  @brief draws entity to the frame buffer

#include "Render.h"
#include "Camera.h"


using namespace kelp;

Render::Render(Mesh * _mesh, Shader * _shader, Texture * _texture, RenderMode _renderMode, kep::Vector3 _colour, bool _disableDepth) : Component()
{
    m_mesh = _mesh;
    m_shader = _shader;
    m_renderMode = _renderMode;
    m_colour = _colour;
    m_texture = _texture;
    m_disableDepth = _disableDepth;

}
Render::~Render()
{
    m_mesh = NULL;
    m_shader = NULL;
    m_transform = NULL;
}

void Render::init()
{
    m_transform = m_owner->getComponent<Transform>();


}
void Render::update()
{
}
void Render::render()
{
    
    glUseProgram(m_shader->m_shaderLocation);

    kep::Matrix4 modMatT = m_transform->m_modelMat.transpose();
    glUniformMatrix4fv(m_shader->m_shaderModelMatLocation, 1, 
                       GL_FALSE, &(modMatT.d[0][0]));
    
    Camera * rc = m_owner->m_world->m_renderCamera;
    glUniformMatrix4fv(m_shader->m_shaderViewMatLocation, 1, 
                       GL_FALSE, &rc->m_viewMat.d[0][0]);
    glUniformMatrix4fv(m_shader->m_shaderProjMatLocation, 1, 
                       GL_FALSE, &rc->m_projectionMat.d[0][0]);
    glUniform3fv(m_shader->m_shaderColourLocation, 1, m_colour.data);
    
    glUniform1i(m_shader->m_shaderText1SamplerLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    if(m_texture != NULL)
    {
        glUniform1i(m_shader->m_shaderTexturedFlagLocation, 1);
        glBindTexture(GL_TEXTURE_2D, m_texture->m_textureLocation);
    }
    else
        glUniform1i(m_shader->m_shaderTexturedFlagLocation, 0);

    switch(m_renderMode)
    {
        case RenderMode::SOLID:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            break;
        
        case RenderMode::WIRE:
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            break;
    }
    if(m_disableDepth) //TODO needs fixing, if the the render componenets is not added las you get some weird stuffe hapening
    {
        glPushAttrib(GL_DEPTH_BUFFER_BIT);//param can just be replaced with GL_DEPTH_BUFFER_BIT but yolo
        //glDepthMask(false);
        glClear(GL_DEPTH_BUFFER_BIT);
    }
        //glDisable(GL_DEPTH_TEST);
    glBindVertexArray( m_mesh->m_vao );
    glDrawArrays(GL_TRIANGLES, 0, m_mesh->m_numVertices);
    glBindVertexArray( 0 );
    if(m_disableDepth)
    {
        //glClear(GL_DEPTH_BUFFER_BIT);
        glPopAttrib();
        //glDepthMask(true);
    }

}
